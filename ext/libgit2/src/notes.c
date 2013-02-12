/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */

#include "notes.h"

#include "git2.h"
#include "refs.h"
#include "config.h"
#include "iterator.h"
#include "signature.h"

static int find_subtree_in_current_level(
	git_tree **out,
	git_repository *repo,
	git_tree *parent,
	const char *annotated_object_sha,
	int fanout)
{
	size_t i;
	const git_tree_entry *entry;

	*out = NULL;

	if (parent == NULL)
		return GIT_ENOTFOUND;

	for (i = 0; i < git_tree_entrycount(parent); i++) {
		entry = git_tree_entry_byindex(parent, i);

		if (!git__ishex(git_tree_entry_name(entry)))
			continue;

		if (S_ISDIR(git_tree_entry_filemode(entry))
			&& strlen(git_tree_entry_name(entry)) == 2
			&& !strncmp(git_tree_entry_name(entry), annotated_object_sha + fanout, 2))
			return git_tree_lookup(out, repo, git_tree_entry_id(entry));

		/* Not a DIR, so do we have an already existing blob? */
		if (!strcmp(git_tree_entry_name(entry), annotated_object_sha + fanout))
			return GIT_EEXISTS;
	}

	return GIT_ENOTFOUND;
}

static int find_subtree_r(git_tree **out, git_tree *root,
			git_repository *repo, const char *target, int *fanout)
{
	int error;
	git_tree *subtree = NULL;

	*out = NULL;

	error = find_subtree_in_current_level(&subtree, repo, root, target, *fanout);
	if (error == GIT_EEXISTS) {
		return git_tree_lookup(out, repo, git_tree_id(root));
	}

	if (error < 0)
		return error;

	*fanout += 2;
	error = find_subtree_r(out, subtree, repo, target, fanout);
	git_tree_free(subtree);

	return error;
}

static int find_blob(git_oid *blob, git_tree *tree, const char *target)
{
	size_t i;
	const git_tree_entry *entry;

	for (i=0; i<git_tree_entrycount(tree); i++) {
		entry = git_tree_entry_byindex(tree, i);

		if (!strcmp(git_tree_entry_name(entry), target)) {
			/* found matching note object - return */

			git_oid_cpy(blob, git_tree_entry_id(entry));
			return 0;
		}
	}
	return GIT_ENOTFOUND;
}

static int tree_write(
	git_tree **out,
	git_repository *repo,
	git_tree *source_tree,
	const git_oid *object_oid,
	const char *treeentry_name,
	unsigned int attributes)
{
	int error;
	git_treebuilder *tb = NULL;
	const git_tree_entry *entry;
	git_oid tree_oid;

	if ((error = git_treebuilder_create(&tb, source_tree)) < 0)
		goto cleanup;

	if (object_oid) {
		if ((error = git_treebuilder_insert(
				&entry, tb, treeentry_name, object_oid, attributes)) < 0)
			goto cleanup;
	} else {
		if ((error = git_treebuilder_remove(tb, treeentry_name)) < 0)
			goto cleanup;
	}

	if ((error = git_treebuilder_write(&tree_oid, repo, tb)) < 0)
		goto cleanup;

	error = git_tree_lookup(out, repo, &tree_oid);

cleanup:
	git_treebuilder_free(tb);
	return error;
}

static int manipulate_note_in_tree_r(
	git_tree **out,
	git_repository *repo,
	git_tree *parent,
	git_oid *note_oid,
	const char *annotated_object_sha,
	int fanout,
	int (*note_exists_cb)(
		git_tree **out,
		git_repository *repo,
		git_tree *parent,
		git_oid *note_oid,
		const char *annotated_object_sha,
		int fanout,
		int current_error),
	int (*note_notfound_cb)(
		git_tree **out,
		git_repository *repo,
		git_tree *parent,
		git_oid *note_oid,
		const char *annotated_object_sha,
		int fanout,
		int current_error))
{
	int error = -1;
	git_tree *subtree = NULL, *new = NULL;
	char subtree_name[3];

	error = find_subtree_in_current_level(
		&subtree, repo, parent, annotated_object_sha, fanout);

	if (error == GIT_EEXISTS) {
		error = note_exists_cb(
			out, repo, parent, note_oid, annotated_object_sha, fanout, error);
		goto cleanup;
	}

	if (error == GIT_ENOTFOUND) {
		error = note_notfound_cb(
			out, repo, parent, note_oid, annotated_object_sha, fanout, error);
		goto cleanup;
	}

	if (error < 0)
		goto cleanup;

	/* An existing fanout has been found, let's dig deeper */
	error = manipulate_note_in_tree_r(
		&new, repo, subtree, note_oid, annotated_object_sha,
		fanout + 2, note_exists_cb, note_notfound_cb);

	if (error < 0)
		goto cleanup;

	strncpy(subtree_name, annotated_object_sha + fanout, 2);
	subtree_name[2] = '\0';

	error = tree_write(out, repo, parent, git_tree_id(new),
			   subtree_name, GIT_FILEMODE_TREE);


cleanup:
	git_tree_free(new);
	git_tree_free(subtree);
	return error;
}

static int remove_note_in_tree_eexists_cb(
	git_tree **out,
	git_repository *repo,
	git_tree *parent,
	git_oid *note_oid,
	const char *annotated_object_sha,
	int fanout,
	int current_error)
{
	GIT_UNUSED(note_oid);
	GIT_UNUSED(current_error);

	return tree_write(out, repo, parent, NULL, annotated_object_sha + fanout, 0);
}

static int remove_note_in_tree_enotfound_cb(
	git_tree **out,
	git_repository *repo,
	git_tree *parent,
	git_oid *note_oid,
	const char *annotated_object_sha,
	int fanout,
	int current_error)
{
	GIT_UNUSED(out);
	GIT_UNUSED(repo);
	GIT_UNUSED(parent);
	GIT_UNUSED(note_oid);
	GIT_UNUSED(fanout);

	giterr_set(GITERR_REPOSITORY, "Object '%s' has no note", annotated_object_sha);
	return current_error;
}

static int insert_note_in_tree_eexists_cb(git_tree **out,
	git_repository *repo,
	git_tree *parent,
	git_oid *note_oid,
	const char *annotated_object_sha,
	int fanout,
	int current_error)
{
	GIT_UNUSED(out);
	GIT_UNUSED(repo);
	GIT_UNUSED(parent);
	GIT_UNUSED(note_oid);
	GIT_UNUSED(fanout);

	giterr_set(GITERR_REPOSITORY, "Note for '%s' exists already", annotated_object_sha);
	return current_error;
}

static int insert_note_in_tree_enotfound_cb(git_tree **out,
	git_repository *repo,
	git_tree *parent,
	git_oid *note_oid,
	const char *annotated_object_sha,
	int fanout,
	int current_error)
{
	GIT_UNUSED(current_error);

	/* No existing fanout at this level, insert in place */
	return tree_write(
		out,
		repo,
		parent,
		note_oid,
		annotated_object_sha + fanout,
		GIT_FILEMODE_BLOB);
}

static int note_write(git_oid *out,
	git_repository *repo,
	const git_signature *author,
	const git_signature *committer,
	const char *notes_ref,
	const char *note,
	git_tree *commit_tree,
	const char *target,
	git_commit **parents,
	int allow_note_overwrite)
{
	int error;
	git_oid oid;
	git_tree *tree = NULL;

	// TODO: should we apply filters?
	/* create note object */
	if ((error = git_blob_create_frombuffer(&oid, repo, note, strlen(note))) < 0)
		goto cleanup;

	if ((error = manipulate_note_in_tree_r(
		&tree, repo, commit_tree, &oid, target, 0,
		allow_note_overwrite ? insert_note_in_tree_enotfound_cb : insert_note_in_tree_eexists_cb,
		insert_note_in_tree_enotfound_cb)) < 0)
		goto cleanup;

	if (out)
		git_oid_cpy(out, &oid);

	error = git_commit_create(&oid, repo, notes_ref, author, committer,
				  NULL, GIT_NOTES_DEFAULT_MSG_ADD,
				  tree, *parents == NULL ? 0 : 1, (const git_commit **) parents);

cleanup:
	git_tree_free(tree);
	return error;
}

static int note_new(git_note **out, git_oid *note_oid, git_blob *blob)
{
	git_note *note = NULL;

	note = (git_note *)git__malloc(sizeof(git_note));
	GITERR_CHECK_ALLOC(note);

	git_oid_cpy(&note->oid, note_oid);
	note->message = git__strdup((char *)git_blob_rawcontent(blob));
	GITERR_CHECK_ALLOC(note->message);

	*out = note;

	return 0;
}

static int note_lookup(git_note **out, git_repository *repo,
		       git_tree *tree, const char *target)
{
	int error, fanout = 0;
	git_oid oid;
	git_blob *blob = NULL;
	git_note *note = NULL;
	git_tree *subtree = NULL;

	if ((error = find_subtree_r(&subtree, tree, repo, target, &fanout)) < 0)
		goto cleanup;

	if ((error = find_blob(&oid, subtree, target + fanout)) < 0)
		goto cleanup;

	if ((error = git_blob_lookup(&blob, repo, &oid)) < 0)
		goto cleanup;

	if ((error = note_new(&note, &oid, blob)) < 0)
		goto cleanup;

	*out = note;

cleanup:
	git_tree_free(subtree);
	git_blob_free(blob);
	return error;
}

static int note_remove(git_repository *repo,
		const git_signature *author, const git_signature *committer,
		const char *notes_ref, git_tree *tree,
		const char *target, git_commit **parents)
{
	int error;
	git_tree *tree_after_removal = NULL;
	git_oid oid;

	if ((error = manipulate_note_in_tree_r(
		&tree_after_removal, repo, tree, NULL, target, 0,
		remove_note_in_tree_eexists_cb, remove_note_in_tree_enotfound_cb)) < 0)
		goto cleanup;

	error = git_commit_create(&oid, repo, notes_ref, author, committer,
	  NULL, GIT_NOTES_DEFAULT_MSG_RM,
	  tree_after_removal,
	  *parents == NULL ? 0 : 1,
	  (const git_commit **) parents);

cleanup:
	git_tree_free(tree_after_removal);
	return error;
}

static int note_get_default_ref(const char **out, git_repository *repo)
{
	int ret;
	git_config *cfg;

	*out = NULL;

	if (git_repository_config__weakptr(&cfg, repo) < 0)
		return -1;

	ret = git_config_get_string(out, cfg, "core.notesRef");
	if (ret == GIT_ENOTFOUND) {
		*out = GIT_NOTES_DEFAULT_REF;
		return 0;
	}

	return ret;
}

static int normalize_namespace(const char **notes_ref, git_repository *repo)
{
	if (*notes_ref)
		return 0;

	return note_get_default_ref(notes_ref, repo);
}

static int retrieve_note_tree_and_commit(
	git_tree **tree_out,
	git_commit **commit_out,
	git_repository *repo,
	const char **notes_ref)
{
	int error;
	git_oid oid;

	if ((error = normalize_namespace(notes_ref, repo)) < 0)
		return error;

	if ((error = git_reference_name_to_id(&oid, repo, *notes_ref)) < 0)
		return error;

	if (git_commit_lookup(commit_out, repo, &oid) < 0)
		return error;

	if ((error = git_commit_tree(tree_out, *commit_out)) < 0)
		return error;

	return 0;
}

int git_note_read(git_note **out, git_repository *repo,
		  const char *notes_ref, const git_oid *oid)
{
	int error;
	char *target = NULL;
	git_tree *tree = NULL;
	git_commit *commit = NULL;

	target = git_oid_allocfmt(oid);
	GITERR_CHECK_ALLOC(target);

	if ((error = retrieve_note_tree_and_commit(&tree, &commit, repo, &notes_ref)) < 0)
		goto cleanup;

	error = note_lookup(out, repo, tree, target);

cleanup:
	git__free(target);
	git_tree_free(tree);
	git_commit_free(commit);
	return error;
}

int git_note_create(
	git_oid *out,
	git_repository *repo,
	const git_signature *author,
	const git_signature *committer,
	const char *notes_ref,
	const git_oid *oid,
	const char *note,
	int allow_note_overwrite)
{
	int error;
	char *target = NULL;
	git_commit *commit = NULL;
	git_tree *tree = NULL;

	target = git_oid_allocfmt(oid);
	GITERR_CHECK_ALLOC(target);

	error = retrieve_note_tree_and_commit(&tree, &commit, repo, &notes_ref);

	if (error < 0 && error != GIT_ENOTFOUND)
		goto cleanup;

	error = note_write(out, repo, author, committer, notes_ref,
			note, tree, target, &commit, allow_note_overwrite);

cleanup:
	git__free(target);
	git_commit_free(commit);
	git_tree_free(tree);
	return error;
}

int git_note_remove(git_repository *repo, const char *notes_ref,
		const git_signature *author, const git_signature *committer,
		const git_oid *oid)
{
	int error;
	char *target = NULL;
	git_commit *commit = NULL;
	git_tree *tree = NULL;

	target = git_oid_allocfmt(oid);
	GITERR_CHECK_ALLOC(target);

	if ((error = retrieve_note_tree_and_commit(&tree, &commit, repo, &notes_ref)) < 0)
		goto cleanup;

	error = note_remove(repo, author, committer, notes_ref,
			    tree, target, &commit);

cleanup:
	git__free(target);
	git_commit_free(commit);
	git_tree_free(tree);
	return error;
}

int git_note_default_ref(const char **out, git_repository *repo)
{
	assert(repo);
	return note_get_default_ref(out, repo);
}

const char * git_note_message(const git_note *note)
{
	assert(note);
	return note->message;
}

const git_oid * git_note_oid(const git_note *note)
{
	assert(note);
	return &note->oid;
}

void git_note_free(git_note *note)
{
	if (note == NULL)
		return;

	git__free(note->message);
	git__free(note);
}

static int process_entry_path(
	const char* entry_path,
	const git_oid *note_oid,
	git_note_foreach_cb note_cb,
	void *payload)
{
	int error = -1;
	size_t i = 0, j = 0, len;
	git_buf buf = GIT_BUF_INIT;
	git_oid annotated_object_id;

	if ((error = git_buf_puts(&buf, entry_path)) < 0)
		goto cleanup;

	len = git_buf_len(&buf);

	while (i < len) {
		if (buf.ptr[i] == '/') {
			i++;
			continue;
		}

		if (git__fromhex(buf.ptr[i]) < 0) {
			/* This is not a note entry */
			goto cleanup;
		}

		if (i != j)
			buf.ptr[j] = buf.ptr[i];

		i++;
		j++;
	}

	buf.ptr[j] = '\0';
	buf.size = j;

	if (j != GIT_OID_HEXSZ) {
		/* This is not a note entry */
		goto cleanup;
	}

	if ((error = git_oid_fromstr(&annotated_object_id, buf.ptr)) < 0)
		goto cleanup;

	if (note_cb(note_oid, &annotated_object_id, payload))
		error = GIT_EUSER;

cleanup:
	git_buf_free(&buf);
	return error;
}

int git_note_foreach(
	git_repository *repo,
	const char *notes_ref,
	git_note_foreach_cb note_cb,
	void *payload)
{
	int error;
	git_iterator *iter = NULL;
	git_tree *tree = NULL;
	git_commit *commit = NULL;
	const git_index_entry *item;

	if (!(error = retrieve_note_tree_and_commit(
			&tree, &commit, repo, &notes_ref)) &&
		!(error = git_iterator_for_tree(&iter, tree)))
		error = git_iterator_current(iter, &item);

	while (!error && item) {
		error = process_entry_path(item->path, &item->oid, note_cb, payload);

		if (!error)
			error = git_iterator_advance(iter, &item);
	}

	git_iterator_free(iter);
	git_tree_free(tree);
	git_commit_free(commit);

	return error;
}
