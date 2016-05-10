# NOTE! NOTE! NOTE!

*Still in testing*

# Trying

Re-create a git svn fetch-able repository easily for anyone wants TortoiseSVN project.

Save my time, and maybe yours, since TortoiseSVN is not a small repository (almost 28000 revisions).

# How

- Clone this repository
- Rename the remote name `origin` to `yhub` (must be this name)
- Run `BkMaster.bat`
- Run `git pack-refs --all`
- Copy all files of `bk_git` folder back to `.git` folder, except the `packed-refs` file
- Merge `bk_git\packed-refs` with `.git\packed-refs`
- Try to run `git svn fetch`

# Memo

- Using `Git Bash` is much faster then `Git CMD` >.<!
  (And Run it on Linux OS will be even much faster than on Windows. XD)
  `git.exe svn clone "svn://svn.code.sf.net/p/tortoisesvn/code/" "D:\TortoiseSvn-git" -T trunk -b branches -t tags`
  **NOTE:** I am using `svn://`, instead of `https://`.
- Run `git config core.filemode false` to get rid of **file mode change 100755 => 100644**

