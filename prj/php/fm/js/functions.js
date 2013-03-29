function checkAll()
{
	for (var i=0;i<this.form.elements.length;i++)
	{
		var e = this.form.elements[i];
		if (e!=this && e.disabled==false)
			e.checked = this.checked;
	}
}
function adjustViewpoint()
{
	var tmp = window.innerHeight - 125;
	if(tmp <= 0)
	{
		tmp = 27;
	}
	/*var isIE = !!window.ActiveXObject;
	var isIE6 = isIE && !window.XMLHttpRequest;
	var isIE8 = isIE && !!document.documentMode;
	var isIE7 = isIE && !isIE6 && !isIE8;
    if ($.browser.msie && $.browser.version < 8) {
        tmp += $.browser.version < 7 ? 12 : 2;
    }*/
	var fileList = document.getElementById("file-list");
	fileList.style.height = tmp + "px";
}
window.onresize = function(){
    adjustViewpoint();
};

var EventUtil={
	addHandler:function(element,type,handler){//添加event
		if(element.addEventListener){
			element.addEventListener(type,handler,false);
		}
		else if(element.attachEvent){
			element.attachEvent("on"+type,handler);
		}
		else {
			element["on"+type]=handler;
		}
	},
	removeHandler:function(element,type,handler){//移除event
		if(element.removeEventListener){
			element.removeEventListener(type,handler,false);
		}
		else if(element.detachEvent){
			element.detachEvent("on"+type,handler);
		}
		else{
			element["on"+type]=null;
		}
	},
	getEvent:function(event){ 
		return event ? event:window.event;
	},
	getTarget:function(event){
		return event.target || event.srcElement;
	},
	preventDefault:function(event){
		if(event.preventDefault){
			event.preventDefault();
		}
		else{
			event.returnValue=false;
		}
	},
	stopPropagation:function(event){
		if(event.stopPropagation){
			event.stopPropagation();
		}
		else{
			event.cancleBubble=true;
		}
	},
	getToTarget:function(event){//mouseout
		if(event.relatedTarget){
			return event.relatedTarget;
		}
		else if(event.toElement){
			return event.toElement;
		}
		else{
			return null;
		}
	},
	getFromTarget:function(event){//mouseover
		if(event.relatedTarget){
			return event.relatedTarget;
		}
		else if(event.fromElement){
			return event.fromElement;
		}
		else{
			return null;
		}		
	},
	getMouseButton:function(event){//哪个鼠标，0,1,2代表左中右
		if(document.implementation.hasFeature("MouseEvents","2.0")){
			return event.button;
		}
		else{
			switch(event.button){
				case 0:
				case 1:
				case 3:
				case 5:
				case 7:return 0;
				case 2:
				case 6:return 2;
				case 4:return 1;
			}			
		}
	},
	getCharCode:function(event){//keypress对应键的ASCLL
		if(typeof event.charCode == "number"){
			return event.charCode;
		}
		else{
			return event.keyCode;
		}
	},
	getWheelDelta:function(event){//mousewheel 和 DOMMouseScroll(firefox)
		if(event.wheelDelta){
		var userAgent=navigator.userAgent.toLowerCase();
			return (window.opera && userAgent.match(/version\/([\d.]+)/)[1]<9.5 ? -event.wheelDelta:event.wheelDelta);
		}
		else{
			return -event.detail*40;//firefox
		}
	},
	createXHR:function(){ //惰性载入
		if(typeof XMLHttpRequest!="undefined"){
			createXHR=function(){
				return new XMLHttpRequest();
			};
		}
		else if(typeof ActiveXObject!="undefined"){
			createXHR=function(){
				if(typeof arguments.callee.activeXString!="string"){
					var versions=["MSXML2.XMLHttp.6.0","MSXML2.XMLHttp.3.0","MSXML2.XMLHttp"];
					for(var i=0,len=versions.length;i<len;i++){
						try{
							var xhr=new ActiveXObject(versions[i]);
							arguments.callee.activeXString=versions[i];
							return xhr;
						}
						catch(ex){//skip
						}
					}			
				}
				return new ActiveXObject(arguments.callee.activeXString);
			};
		}
		else{
			createXHR=function(){
				throw new Error("No XHR object available.");
			};
		}
	}
}

function hasClassName(element,className)
{
	var classNames = element.className.replace(/\s+/,' ').split(' ')
	for(var i = 0; i < classNames.length; i++){
		if(classNames[i] === className) return true;
	}
	return false;
}

function getFileList(event)
{
	//var path = document.getElementById('path');
	//path.value = this.href.replace(location.href,'');	
	var path = this.href.replace(location.href,'');	
	var xhr = createXMLHttpRequest();
	
	xhr.open('POST','index.php',true);
	xhr.onreadystatechange = flushFileList;
	xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xhr.send('path='+path);		
	this.href = "javascript:void(0)";
}

function flushFileList()
{
	if(this.readyState == 4 && this.status == 200)
	{			
		var filecontainer = document.getElementById('file-container');
		filecontainer.innerHTML = this.responseText;
		doit();
	}
}

function addAnchorClickEvent(parent)
{
	var links = parent.getElementsByTagName('a');
	for(var i=0;i<links.length;i++)
	{
		if(hasClassName(links[i],'ajaxPath'))
		{
			addEventListener(links[i],'click',getFileList);
		}
	}
}

function addEventListener(element,event,listener)
{
	if(element.addEventListener){
		element.addEventListener(event,listener,false);
		return true;
	}
	else if(element.attachEvent){
		element.attachEvent('on' + event,listener);
		return true;
	}
	return false;
}

function createXMLHttpRequest()
{
	var xhr = false;
	if(window.XMLHttpRequest){
		xhr = new XMLHttpRequest();
	}
	else if(window.ActiveXObject){
		xhr = new ActiveXObject();
	}
	return xhr;
}