(function(e,t,n,i){"use strict";var o=n(e),a=n(t),r=n.fancybox=function(){r.open.apply(this,arguments)},l=navigator.userAgent.match(/msie/i),s=null,c=t.createTouch!==i,d=function(e){return e&&e.hasOwnProperty&&e instanceof n},h=function(e){return e&&"string"===n.type(e)},p=function(e){return h(e)&&e.indexOf("%")>0},f=function(e){return e&&!(e.style.overflow&&"hidden"===e.style.overflow)&&(e.clientWidth&&e.scrollWidth>e.clientWidth||e.clientHeight&&e.scrollHeight>e.clientHeight)},u=function(e,t){var n=parseInt(e,10)||0;return t&&p(e)&&(n=r.getViewport()[t]/100*n),Math.ceil(n)},g=function(e,t){return u(e,t)+"px"};n.extend(r,{version:"2.1.4",defaults:{padding:15,margin:20,width:800,height:600,minWidth:100,minHeight:100,maxWidth:9999,maxHeight:9999,autoSize:!0,autoHeight:!1,autoWidth:!1,autoResize:!0,autoCenter:!c,fitToView:!0,aspectRatio:!1,topRatio:.5,leftRatio:.5,scrolling:"auto",wrapCSS:"",arrows:!0,closeBtn:!0,closeClick:!1,nextClick:!1,mouseWheel:!0,autoPlay:!1,playSpeed:3e3,preload:3,modal:!1,loop:!0,ajax:{dataType:"html",headers:{"X-fancyBox":!0}},iframe:{scrolling:"auto",preload:!0},swf:{wmode:"transparent",allowfullscreen:"true",allowscriptaccess:"always"},keys:{next:{13:"left",34:"up",39:"left",40:"up"},prev:{8:"right",33:"down",37:"right",38:"down"},close:[27],play:[32],toggle:[70]},direction:{next:"left",prev:"right"},scrollOutside:!0,index:0,type:null,href:null,content:null,title:null,tpl:{wrap:'<div class="fancybox-wrap" tabIndex="-1"><div class="fancybox-skin"><div class="fancybox-outer"><div class="fancybox-inner"></div></div></div></div>',image:'<img class="fancybox-image" src="{href}" alt="" />',iframe:'<iframe id="fancybox-frame{rnd}" name="fancybox-frame{rnd}" class="fancybox-iframe" frameborder="0" vspace="0" hspace="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen'+(l?' allowtransparency="true"':"")+"></iframe>",error:'<p class="fancybox-error">The requested content cannot be loaded.<br/>Please try again later.</p>',closeBtn:'<a title="Close" class="fancybox-item fancybox-close" href="javascript:;"></a>',next:'<a title="Next" class="fancybox-nav fancybox-next" href="javascript:;"><span></span></a>',prev:'<a title="Previous" class="fancybox-nav fancybox-prev" href="javascript:;"><span></span></a>'},openEffect:"fade",openSpeed:250,openEasing:"swing",openOpacity:!0,openMethod:"zoomIn",closeEffect:"fade",closeSpeed:250,closeEasing:"swing",closeOpacity:!0,closeMethod:"zoomOut",nextEffect:"elastic",nextSpeed:250,nextEasing:"swing",nextMethod:"changeIn",prevEffect:"elastic",prevSpeed:250,prevEasing:"swing",prevMethod:"changeOut",helpers:{overlay:!0,title:!0},onCancel:n.noop,beforeLoad:n.noop,afterLoad:n.noop,beforeShow:n.noop,afterShow:n.noop,beforeChange:n.noop,beforeClose:n.noop,afterClose:n.noop},group:{},opts:{},previous:null,coming:null,current:null,isActive:!1,isOpen:!1,isOpened:!1,wrap:null,skin:null,outer:null,inner:null,player:{timer:null,isActive:!1},ajaxLoad:null,imgPreload:null,transitions:{},helpers:{},open:function(e,t){return e&&(n.isPlainObject(t)||(t={}),!1!==r.close(!0))?(n.isArray(e)||(e=d(e)?n(e).get():[e]),n.each(e,function(o,a){var l,s,c,p,f,u,g,m={};"object"===n.type(a)&&(a.nodeType&&(a=n(a)),d(a)?(m={href:a.data("fancybox-href")||a.attr("href"),title:a.data("fancybox-title")||a.attr("title"),isDom:!0,element:a},n.metadata&&n.extend(!0,m,a.metadata())):m=a),l=t.href||m.href||(h(a)?a:null),s=t.title!==i?t.title:m.title||"",c=t.content||m.content,p=c?"html":t.type||m.type,!p&&m.isDom&&(p=a.data("fancybox-type"),p||(f=a.prop("class").match(/fancybox\.(\w+)/),p=f?f[1]:null)),h(l)&&(p||(r.isImage(l)?p="image":r.isSWF(l)?p="swf":"#"===l.charAt(0)?p="inline":h(a)&&(p="html",c=a)),"ajax"===p&&(u=l.split(/\s+/,2),l=u.shift(),g=u.shift())),c||("inline"===p?l?c=n(h(l)?l.replace(/.*(?=#[^\s]+$)/,""):l):m.isDom&&(c=a):"html"===p?c=l:p||l||!m.isDom||(p="inline",c=a)),n.extend(m,{href:l,type:p,content:c,title:s,selector:g}),e[o]=m}),r.opts=n.extend(!0,{},r.defaults,t),t.keys!==i&&(r.opts.keys=t.keys?n.extend({},r.defaults.keys,t.keys):!1),r.group=e,r._start(r.opts.index)):i},cancel:function(){var e=r.coming;e&&!1!==r.trigger("onCancel")&&(r.hideLoading(),r.ajaxLoad&&r.ajaxLoad.abort(),r.ajaxLoad=null,r.imgPreload&&(r.imgPreload.onload=r.imgPreload.onerror=null),e.wrap&&e.wrap.stop(!0,!0).trigger("onReset").remove(),r.coming=null,r.current||r._afterZoomOut(e))},close:function(e){r.cancel(),!1!==r.trigger("beforeClose")&&(r.unbindEvents(),r.isActive&&(r.isOpen&&e!==!0?(r.isOpen=r.isOpened=!1,r.isClosing=!0,n(".fancybox-item, .fancybox-nav").remove(),r.wrap.stop(!0,!0).removeClass("fancybox-opened"),r.transitions[r.current.closeMethod]()):(n(".fancybox-wrap").stop(!0).trigger("onReset").remove(),r._afterZoomOut())))},play:function(e){var t=function(){clearTimeout(r.player.timer)},n=function(){t(),r.current&&r.player.isActive&&(r.player.timer=setTimeout(r.next,r.current.playSpeed))},i=function(){t(),a.unbind(".player"),r.player.isActive=!1,r.trigger("onPlayEnd")},o=function(){r.current&&(r.current.loop||r.current.index<r.group.length-1)&&(r.player.isActive=!0,a.bind({"onCancel.player beforeClose.player":i,"onUpdate.player":n,"beforeLoad.player":t}),n(),r.trigger("onPlayStart"))};e===!0||!r.player.isActive&&e!==!1?o():i()},next:function(e){var t=r.current;t&&(h(e)||(e=t.direction.next),r.jumpto(t.index+1,e,"next"))},prev:function(e){var t=r.current;t&&(h(e)||(e=t.direction.prev),r.jumpto(t.index-1,e,"prev"))},jumpto:function(e,t,n){var o=r.current;o&&(e=u(e),r.direction=t||o.direction[e>=o.index?"next":"prev"],r.router=n||"jumpto",o.loop&&(0>e&&(e=o.group.length+e%o.group.length),e%=o.group.length),o.group[e]!==i&&(r.cancel(),r._start(e)))},reposition:function(e,t){var i,o=r.current,a=o?o.wrap:null;a&&(i=r._getPosition(t),e&&"scroll"===e.type?(delete i.position,a.stop(!0,!0).animate(i,200)):(a.css(i),o.pos=n.extend({},o.dim,i)))},update:function(e){var t=e&&e.type,n=!t||"orientationchange"===t;n&&(clearTimeout(s),s=null),r.isOpen&&!s&&(s=setTimeout(function(){var i=r.current;i&&!r.isClosing&&(r.wrap.removeClass("fancybox-tmp"),(n||"load"===t||"resize"===t&&i.autoResize)&&r._setDimension(),"scroll"===t&&i.canShrink||r.reposition(e),r.trigger("onUpdate"),s=null)},n&&!c?0:300))},toggle:function(e){r.isOpen&&(r.current.fitToView="boolean"===n.type(e)?e:!r.current.fitToView,c&&(r.wrap.removeAttr("style").addClass("fancybox-tmp"),r.trigger("onUpdate")),r.update())},hideLoading:function(){a.unbind(".loading"),n("#fancybox-loading").remove()},showLoading:function(){var e,t;r.hideLoading(),e=n('<div id="fancybox-loading"><div></div></div>').click(r.cancel).appendTo("body"),a.bind("keydown.loading",function(e){27===(e.which||e.keyCode)&&(e.preventDefault(),r.cancel())}),r.defaults.fixed||(t=r.getViewport(),e.css({position:"absolute",top:.5*t.h+t.y,left:.5*t.w+t.x}))},getViewport:function(){var t=r.current&&r.current.locked||!1,n={x:o.scrollLeft(),y:o.scrollTop()};return t?(n.w=t[0].clientWidth,n.h=t[0].clientHeight):(n.w=c&&e.innerWidth?e.innerWidth:o.width(),n.h=c&&e.innerHeight?e.innerHeight:o.height()),n},unbindEvents:function(){r.wrap&&d(r.wrap)&&r.wrap.unbind(".fb"),a.unbind(".fb"),o.unbind(".fb")},bindEvents:function(){var e,t=r.current;t&&(o.bind("orientationchange.fb"+(c?"":" resize.fb")+(t.autoCenter&&!t.locked?" scroll.fb":""),r.update),e=t.keys,e&&a.bind("keydown.fb",function(o){var a=o.which||o.keyCode,l=o.target||o.srcElement;return 27===a&&r.coming?!1:(o.ctrlKey||o.altKey||o.shiftKey||o.metaKey||l&&(l.type||n(l).is("[contenteditable]"))||n.each(e,function(e,l){return t.group.length>1&&l[a]!==i?(r[e](l[a]),o.preventDefault(),!1):n.inArray(a,l)>-1?(r[e](),o.preventDefault(),!1):i}),i)}),n.fn.mousewheel&&t.mouseWheel&&r.wrap.bind("mousewheel.fb",function(e,i,o,a){for(var l=e.target||null,s=n(l),c=!1;s.length&&!(c||s.is(".fancybox-skin")||s.is(".fancybox-wrap"));)c=f(s[0]),s=n(s).parent();0===i||c||r.group.length>1&&!t.canShrink&&(a>0||o>0?r.prev(a>0?"down":"left"):(0>a||0>o)&&r.next(0>a?"up":"right"),e.preventDefault())}))},trigger:function(e,t){var i,o=t||r.coming||r.current;if(o){if(n.isFunction(o[e])&&(i=o[e].apply(o,Array.prototype.slice.call(arguments,1))),i===!1)return!1;o.helpers&&n.each(o.helpers,function(t,i){i&&r.helpers[t]&&n.isFunction(r.helpers[t][e])&&(i=n.extend(!0,{},r.helpers[t].defaults,i),r.helpers[t][e](i,o))}),a.trigger(e)}},isImage:function(e){return h(e)&&e.match(/(^data:image\/.*,)|(\.(jp(e|g|eg)|gif|png|bmp|webp)((\?|#).*)?$)/i)},isSWF:function(e){return h(e)&&e.match(/\.(swf)((\?|#).*)?$/i)},_start:function(e){var t,o,a,l,s,d={};if(e=u(e),t=r.group[e]||null,!t)return!1;if(d=n.extend(!0,{},r.opts,t),l=d.margin,s=d.padding,"number"===n.type(l)&&(d.margin=[l,l,l,l]),"number"===n.type(s)&&(d.padding=[s,s,s,s]),d.modal&&n.extend(!0,d,{closeBtn:!1,closeClick:!1,nextClick:!1,arrows:!1,mouseWheel:!1,keys:null,helpers:{overlay:{closeClick:!1}}}),d.autoSize&&(d.autoWidth=d.autoHeight=!0),"auto"===d.width&&(d.autoWidth=!0),"auto"===d.height&&(d.autoHeight=!0),d.group=r.group,d.index=e,r.coming=d,!1===r.trigger("beforeLoad"))return r.coming=null,i;if(a=d.type,o=d.href,!a)return r.coming=null,r.current&&r.router&&"jumpto"!==r.router?(r.current.index=e,r[r.router](r.direction)):!1;if(r.isActive=!0,("image"===a||"swf"===a)&&(d.autoHeight=d.autoWidth=!1,d.scrolling="visible"),"image"===a&&(d.aspectRatio=!0),"iframe"===a&&c&&(d.scrolling="scroll"),d.wrap=n(d.tpl.wrap).addClass("fancybox-"+(c?"mobile":"desktop")+" fancybox-type-"+a+" fancybox-tmp "+d.wrapCSS).appendTo(d.parent||"body"),n.extend(d,{skin:n(".fancybox-skin",d.wrap),outer:n(".fancybox-outer",d.wrap),inner:n(".fancybox-inner",d.wrap)}),n.each(["Top","Right","Bottom","Left"],function(e,t){d.skin.css("padding"+t,g(d.padding[e]))}),r.trigger("onReady"),"inline"===a||"html"===a){if(!d.content||!d.content.length)return r._error("content")}else if(!o)return r._error("href");"image"===a?r._loadImage():"ajax"===a?r._loadAjax():"iframe"===a?r._loadIframe():r._afterLoad()},_error:function(e){n.extend(r.coming,{type:"html",autoWidth:!0,autoHeight:!0,minWidth:0,minHeight:0,scrolling:"no",hasError:e,content:r.coming.tpl.error}),r._afterLoad()},_loadImage:function(){var e=r.imgPreload=new Image;e.onload=function(){this.onload=this.onerror=null,r.coming.width=this.width,r.coming.height=this.height,r._afterLoad()},e.onerror=function(){this.onload=this.onerror=null,r._error("image")},e.src=r.coming.href,e.complete!==!0&&r.showLoading()},_loadAjax:function(){var e=r.coming;r.showLoading(),r.ajaxLoad=n.ajax(n.extend({},e.ajax,{url:e.href,error:function(e,t){r.coming&&"abort"!==t?r._error("ajax",e):r.hideLoading()},success:function(t,n){"success"===n&&(e.content=t,r._afterLoad())}}))},_loadIframe:function(){var e=r.coming,t=n(e.tpl.iframe.replace(/\{rnd\}/g,(new Date).getTime())).attr("scrolling",c?"auto":e.iframe.scrolling).attr("src",e.href);n(e.wrap).bind("onReset",function(){try{n(this).find("iframe").hide().attr("src","//about:blank").end().empty()}catch(e){}}),e.iframe.preload&&(r.showLoading(),t.one("load",function(){n(this).data("ready",1),c||n(this).bind("load.fb",r.update),n(this).parents(".fancybox-wrap").width("100%").removeClass("fancybox-tmp").show(),r._afterLoad()})),e.content=t.appendTo(e.inner),e.iframe.preload||r._afterLoad()},_preloadImages:function(){var e,t,n=r.group,i=r.current,o=n.length,a=i.preload?Math.min(i.preload,o-1):0;for(t=1;a>=t;t+=1)e=n[(i.index+t)%o],"image"===e.type&&e.href&&((new Image).src=e.href)},_afterLoad:function(){var e,t,o,a,l,s,c=r.coming,h=r.current,p="fancybox-placeholder";if(r.hideLoading(),c&&r.isActive!==!1){if(!1===r.trigger("afterLoad",c,h))return c.wrap.stop(!0).trigger("onReset").remove(),r.coming=null,i;switch(h&&(r.trigger("beforeChange",h),h.wrap.stop(!0).removeClass("fancybox-opened").find(".fancybox-item, .fancybox-nav").remove()),r.unbindEvents(),e=c,t=c.content,o=c.type,a=c.scrolling,n.extend(r,{wrap:e.wrap,skin:e.skin,outer:e.outer,inner:e.inner,current:e,previous:h}),l=e.href,o){case"inline":case"ajax":case"html":e.selector?t=n("<div>").html(t).find(e.selector):d(t)&&(t.data(p)||t.data(p,n('<div class="'+p+'"></div>').insertAfter(t).hide()),t=t.show().detach(),e.wrap.bind("onReset",function(){n(this).find(t).length&&t.hide().replaceAll(t.data(p)).data(p,!1)}));break;case"image":t=e.tpl.image.replace("{href}",l);break;case"swf":t='<object id="fancybox-swf" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="100%" height="100%"><param name="movie" value="'+l+'"></param>',s="",n.each(e.swf,function(e,n){t+='<param name="'+e+'" value="'+n+'"></param>',s+=" "+e+'="'+n+'"'}),t+='<embed src="'+l+'" type="application/x-shockwave-flash" width="100%" height="100%"'+s+"></embed></object>"}d(t)&&t.parent().is(e.inner)||e.inner.append(t),r.trigger("beforeShow"),e.inner.css("overflow","yes"===a?"scroll":"no"===a?"hidden":a),r._setDimension(),r.reposition(),r.isOpen=!1,r.coming=null,r.bindEvents(),r.isOpened?h.prevMethod&&r.transitions[h.prevMethod]():n(".fancybox-wrap").not(e.wrap).stop(!0).trigger("onReset").remove(),r.transitions[r.isOpened?e.nextMethod:e.openMethod](),r._preloadImages()}},_setDimension:function(){var e,t,i,o,a,l,s,c,d,h,f,m,y,w,v,x=r.getViewport(),b=0,k=!1,C=!1,M=r.wrap,S=r.skin,O=r.inner,W=r.current,_=W.width,E=W.height,L=W.minWidth,j=W.minHeight,T=W.maxWidth,H=W.maxHeight,P=W.scrolling,D=W.scrollOutside?W.scrollbarWidth:0,A=W.margin,R=u(A[1]+A[3]),I=u(A[0]+A[2]);if(M.add(S).add(O).width("auto").height("auto").removeClass("fancybox-tmp"),e=u(S.outerWidth(!0)-S.width()),t=u(S.outerHeight(!0)-S.height()),i=R+e,o=I+t,a=p(_)?(x.w-i)*u(_)/100:_,l=p(E)?(x.h-o)*u(E)/100:E,"iframe"===W.type){if(w=W.content,W.autoHeight&&1===w.data("ready"))try{w[0].contentWindow.document.location&&(O.width(a).height(9999),v=w.contents().find("body"),D&&v.css("overflow-x","hidden"),l=v.height())}catch(z){}}else(W.autoWidth||W.autoHeight)&&(O.addClass("fancybox-tmp"),W.autoWidth||O.width(a),W.autoHeight||O.height(l),W.autoWidth&&(a=O.width()),W.autoHeight&&(l=O.height()),O.removeClass("fancybox-tmp"));if(_=u(a),E=u(l),d=a/l,L=u(p(L)?u(L,"w")-i:L),T=u(p(T)?u(T,"w")-i:T),j=u(p(j)?u(j,"h")-o:j),H=u(p(H)?u(H,"h")-o:H),s=T,c=H,W.fitToView&&(T=Math.min(x.w-i,T),H=Math.min(x.h-o,H)),m=x.w-R,y=x.h-I,W.aspectRatio?(_>T&&(_=T,E=u(_/d)),E>H&&(E=H,_=u(E*d)),L>_&&(_=L,E=u(_/d)),j>E&&(E=j,_=u(E*d))):(_=Math.max(L,Math.min(_,T)),W.autoHeight&&"iframe"!==W.type&&(O.width(_),E=O.height()),E=Math.max(j,Math.min(E,H))),W.fitToView)if(O.width(_).height(E),M.width(_+e),h=M.width(),f=M.height(),W.aspectRatio)for(;(h>m||f>y)&&_>L&&E>j&&!(b++>19);)E=Math.max(j,Math.min(H,E-10)),_=u(E*d),L>_&&(_=L,E=u(_/d)),_>T&&(_=T,E=u(_/d)),O.width(_).height(E),M.width(_+e),h=M.width(),f=M.height();else _=Math.max(L,Math.min(_,_-(h-m))),E=Math.max(j,Math.min(E,E-(f-y)));D&&"auto"===P&&l>E&&m>_+e+D&&(_+=D),O.width(_).height(E),M.width(_+e),h=M.width(),f=M.height(),k=(h>m||f>y)&&_>L&&E>j,C=W.aspectRatio?s>_&&c>E&&a>_&&l>E:(s>_||c>E)&&(a>_||l>E),n.extend(W,{dim:{width:g(h),height:g(f)},origWidth:a,origHeight:l,canShrink:k,canExpand:C,wPadding:e,hPadding:t,wrapSpace:f-S.outerHeight(!0),skinSpace:S.height()-E}),!w&&W.autoHeight&&E>j&&H>E&&!C&&O.height("auto")},_getPosition:function(e){var t=r.current,n=r.getViewport(),i=t.margin,o=r.wrap.width()+i[1]+i[3],a=r.wrap.height()+i[0]+i[2],l={position:"absolute",top:i[0],left:i[3]};return t.autoCenter&&t.fixed&&!e&&n.h>=a&&n.w>=o?l.position="fixed":t.locked||(l.top+=n.y,l.left+=n.x),l.top=g(Math.max(l.top,l.top+(n.h-a)*t.topRatio)),l.left=g(Math.max(l.left,l.left+(n.w-o)*t.leftRatio)),l},_afterZoomIn:function(){var e=r.current;e&&(r.isOpen=r.isOpened=!0,r.wrap.css("overflow","visible").addClass("fancybox-opened"),r.update(),(e.closeClick||e.nextClick&&r.group.length>1)&&r.inner.css("cursor","pointer").bind("click.fb",function(t){n(t.target).is("a")||n(t.target).parent().is("a")||(t.preventDefault(),r[e.closeClick?"close":"next"]())}),e.closeBtn&&n(e.tpl.closeBtn).appendTo(r.skin).bind("click.fb",function(e){e.preventDefault(),r.close()}),e.arrows&&r.group.length>1&&((e.loop||e.index>0)&&n(e.tpl.prev).appendTo(r.outer).bind("click.fb",r.prev),(e.loop||e.index<r.group.length-1)&&n(e.tpl.next).appendTo(r.outer).bind("click.fb",r.next)),r.trigger("afterShow"),e.loop||e.index!==e.group.length-1?r.opts.autoPlay&&!r.player.isActive&&(r.opts.autoPlay=!1,r.play()):r.play(!1))},_afterZoomOut:function(e){e=e||r.current,n(".fancybox-wrap").trigger("onReset").remove(),n.extend(r,{group:{},opts:{},router:!1,current:null,isActive:!1,isOpened:!1,isOpen:!1,isClosing:!1,wrap:null,skin:null,outer:null,inner:null}),r.trigger("afterClose",e)}}),r.transitions={getOrigPosition:function(){var e=r.current,t=e.element,n=e.orig,i={},o=50,a=50,l=e.hPadding,s=e.wPadding,c=r.getViewport();return!n&&e.isDom&&t.is(":visible")&&(n=t.find("img:first"),n.length||(n=t)),d(n)?(i=n.offset(),n.is("img")&&(o=n.outerWidth(),a=n.outerHeight())):(i.top=c.y+(c.h-a)*e.topRatio,i.left=c.x+(c.w-o)*e.leftRatio),("fixed"===r.wrap.css("position")||e.locked)&&(i.top-=c.y,i.left-=c.x),i={top:g(i.top-l*e.topRatio),left:g(i.left-s*e.leftRatio),width:g(o+s),height:g(a+l)}},step:function(e,t){var n,i,o,a=t.prop,l=r.current,s=l.wrapSpace,c=l.skinSpace;("width"===a||"height"===a)&&(n=t.end===t.start?1:(e-t.start)/(t.end-t.start),r.isClosing&&(n=1-n),i="width"===a?l.wPadding:l.hPadding,o=e-i,r.skin[a](u("width"===a?o:o-s*n)),r.inner[a](u("width"===a?o:o-s*n-c*n)))},zoomIn:function(){var e=r.current,t=e.pos,i=e.openEffect,o="elastic"===i,a=n.extend({opacity:1},t);delete a.position,o?(t=this.getOrigPosition(),e.openOpacity&&(t.opacity=.1)):"fade"===i&&(t.opacity=.1),r.wrap.css(t).animate(a,{duration:"none"===i?0:e.openSpeed,easing:e.openEasing,step:o?this.step:null,complete:r._afterZoomIn})},zoomOut:function(){var e=r.current,t=e.closeEffect,n="elastic"===t,i={opacity:.1};n&&(i=this.getOrigPosition(),e.closeOpacity&&(i.opacity=.1)),r.wrap.animate(i,{duration:"none"===t?0:e.closeSpeed,easing:e.closeEasing,step:n?this.step:null,complete:r._afterZoomOut})},changeIn:function(){var e,t=r.current,n=t.nextEffect,i=t.pos,o={opacity:1},a=r.direction,l=200;i.opacity=.1,"elastic"===n&&(e="down"===a||"up"===a?"top":"left","down"===a||"right"===a?(i[e]=g(u(i[e])-l),o[e]="+="+l+"px"):(i[e]=g(u(i[e])+l),o[e]="-="+l+"px")),"none"===n?r._afterZoomIn():r.wrap.css(i).animate(o,{duration:t.nextSpeed,easing:t.nextEasing,complete:r._afterZoomIn})},changeOut:function(){var e=r.previous,t=e.prevEffect,i={opacity:.1},o=r.direction,a=200;"elastic"===t&&(i["down"===o||"up"===o?"top":"left"]=("up"===o||"left"===o?"-":"+")+"="+a+"px"),e.wrap.animate(i,{duration:"none"===t?0:e.prevSpeed,easing:e.prevEasing,complete:function(){n(this).trigger("onReset").remove()}})}},r.helpers.overlay={defaults:{closeClick:!0,speedOut:200,showEarly:!0,css:{},locked:!c,fixed:!0},overlay:null,fixed:!1,create:function(e){e=n.extend({},this.defaults,e),this.overlay&&this.close(),this.overlay=n('<div class="fancybox-overlay"></div>').appendTo("body"),this.fixed=!1,e.fixed&&r.defaults.fixed&&(this.overlay.addClass("fancybox-overlay-fixed"),this.fixed=!0)},open:function(e){var t=this;e=n.extend({},this.defaults,e),this.overlay?this.overlay.unbind(".overlay").width("auto").height("auto"):this.create(e),this.fixed||(o.bind("resize.overlay",n.proxy(this.update,this)),this.update()),e.closeClick&&this.overlay.bind("click.overlay",function(e){n(e.target).hasClass("fancybox-overlay")&&(r.isActive?r.close():t.close())}),this.overlay.css(e.css).show()},close:function(){n(".fancybox-overlay").remove(),o.unbind("resize.overlay"),this.overlay=null,this.margin!==!1&&(n("body").css("margin-right",this.margin),this.margin=!1),this.el&&this.el.removeClass("fancybox-lock")},update:function(){var e,n="100%";this.overlay.width(n).height("100%"),l?(e=Math.max(t.documentElement.offsetWidth,t.body.offsetWidth),a.width()>e&&(n=a.width())):a.width()>o.width()&&(n=a.width()),this.overlay.width(n).height(a.height())},onReady:function(e,i){n(".fancybox-overlay").stop(!0,!0),this.overlay||(this.margin=a.height()>o.height()||"scroll"===n("body").css("overflow-y")?n("body").css("margin-right"):!1,this.el=t.all&&!t.querySelector?n("html"):n("body"),this.create(e)),e.locked&&this.fixed&&(i.locked=this.overlay.append(i.wrap),i.fixed=!1),e.showEarly===!0&&this.beforeShow.apply(this,arguments)},beforeShow:function(e,t){t.locked&&(this.el.addClass("fancybox-lock"),this.margin!==!1&&n("body").css("margin-right",u(this.margin)+t.scrollbarWidth)),this.open(e)},onUpdate:function(){this.fixed||this.update()},afterClose:function(e){this.overlay&&!r.isActive&&this.overlay.fadeOut(e.speedOut,n.proxy(this.close,this))}},r.helpers.title={defaults:{type:"float",position:"bottom"},beforeShow:function(e){var t,i,o=r.current,a=o.title,s=e.type;if(n.isFunction(a)&&(a=a.call(o.element,o)),h(a)&&""!==n.trim(a)){switch(t=n('<div class="fancybox-title fancybox-title-'+s+'-wrap">'+a+"</div>"),s){case"inside":i=r.skin;break;case"outside":i=r.wrap;break;case"over":i=r.inner;break;default:i=r.skin,t.appendTo("body"),l&&t.width(t.width()),t.wrapInner('<span class="child"></span>'),r.current.margin[2]+=Math.abs(u(t.css("margin-bottom")))}t["top"===e.position?"prependTo":"appendTo"](i)}}},n.fn.fancybox=function(e){var t,i=n(this),o=this.selector||"",l=function(a){var l,s,c=n(this).blur(),d=t;a.ctrlKey||a.altKey||a.shiftKey||a.metaKey||c.is(".fancybox-wrap")||(l=e.groupAttr||"data-fancybox-group",s=c.attr(l),s||(l="rel",s=c.get(0)[l]),s&&""!==s&&"nofollow"!==s&&(c=o.length?n(o):i,c=c.filter("["+l+'="'+s+'"]'),d=c.index(this)),e.index=d,r.open(c,e)!==!1&&a.preventDefault())};return e=e||{},t=e.index||0,o&&e.live!==!1?a.undelegate(o,"click.fb-start").delegate(o+":not('.fancybox-item, .fancybox-nav')","click.fb-start",l):i.unbind("click.fb-start").bind("click.fb-start",l),this.filter("[data-fancybox-start=1]").trigger("click"),this},a.ready(function(){n.scrollbarWidth===i&&(n.scrollbarWidth=function(){var e=n('<div style="width:50px;height:50px;overflow:auto"><div/></div>').appendTo("body"),t=e.children(),i=t.innerWidth()-t.height(99).innerWidth();return e.remove(),i}),n.support.fixedPosition===i&&(n.support.fixedPosition=function(){var e=n('<div style="position:fixed;top:20px;"></div>').appendTo("body"),t=20===e[0].offsetTop||15===e[0].offsetTop;return e.remove(),t}()),n.extend(r.defaults,{scrollbarWidth:n.scrollbarWidth(),fixed:n.support.fixedPosition,parent:n("body")})})})(window,document,jQuery),function(e){"function"==typeof define&&define.amd?define(["jquery"],e):e(jQuery)}(function(e){function t(t){var o=t||window.event,a=[].slice.call(arguments,1),r=0,l=0,s=0,c=0,d=0;return t=e.event.fix(o),t.type="mousewheel",o.wheelDelta&&(r=o.wheelDelta),o.detail&&(r=-1*o.detail),o.deltaY&&(s=-1*o.deltaY,r=s),o.deltaX&&(l=o.deltaX,r=-1*l),void 0!==o.wheelDeltaY&&(s=o.wheelDeltaY),void 0!==o.wheelDeltaX&&(l=-1*o.wheelDeltaX),c=Math.abs(r),(!n||n>c)&&(n=c),d=Math.max(Math.abs(s),Math.abs(l)),(!i||i>d)&&(i=d),a.unshift(t,Math.floor(r/n),Math.floor(l/i),Math.floor(s/i)),(e.event.dispatch||e.event.handle).apply(this,a)}var n,i,o=["wheel","mousewheel","DOMMouseScroll"],a="onwheel"in document||document.documentMode>=9?["wheel"]:["mousewheel","DomMouseScroll","MozMousePixelScroll"];if(e.event.fixHooks)for(var r=o.length;r;)e.event.fixHooks[o[--r]]=e.event.mouseHooks;e.event.special.mousewheel={setup:function(){if(this.addEventListener)for(var e=a.length;e;)this.addEventListener(a[--e],t,!1);else this.onmousewheel=t},teardown:function(){if(this.removeEventListener)for(var e=a.length;e;)this.removeEventListener(a[--e],t,!1);else this.onmousewheel=null}},e.fn.extend({mousewheel:function(e){return e?this.bind("mousewheel",e):this.trigger("mousewheel")},unmousewheel:function(e){return this.unbind("mousewheel",e)}})});