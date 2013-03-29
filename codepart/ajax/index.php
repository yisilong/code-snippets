<!doctype html>
<html>
<head>
</head>
<body>
<button id="submit1" name="submit1">GET-TEXT</button>
<button id="submit2" name="submit2">GET-XML</Button>
<button id="submit3" name="submit3">POST-TEXT</button>
<button id="submit4" name="submit4">POST-XML</button>
<div id="ajax">it well be changes!</div>
<script type="text/javascript">
//$
function $(id)
{
	return document.getElementById(id);
}
//add event to element
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
//create XMLHttpRequest
function createXMLHttpRequest(func,meothod,url,query)
{
	var xhr = false;
	if(window.XMLHttpRequest){
		xhr = new XMLHttpRequest();
	}
	else if(window.ActiveXObject){
		xhr = new ActiveXObject();
	}
	if(xhr){
		if(meothod === 'GET'){
			xhr.open('GET',url+'?'+query,true);
			xhr.send(null);
		}
		else if(meothod === 'POST'){			
			xhr.open('POST',url,true);
			xhr.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
			xhr.send(query);
		}
	xhr.onreadystatechange = func;
	}
	return xhr;
}
function listener(event)
{
	var value = this.innerHTML;
	var arr = value.split('-');
	var xhr=createXMLHttpRequest(process,arr[0],'ajax.php',arr.join('='));
}
//一个获取元素文本内容的通用函数 
function text(e){ 
var t=""; 
e=e.childNodes || e;//如果传入的是元素，则继续遍历其子元素；否则假定它是一个数组 
for(var i=0;i<e.length;i++){ 
//如果不是元素，追回其文本值； 
//否则，递归遍历所有元素的子节点； 
t += e[i].nodeType != 1 ? e[i].nodeValue : text(e[i].childNodes); 
} 
return t; 
} 
function process()
{
	if(this.readyState == 4 && this.status == 200)
	{

		var contentType = this.getResponseHeader('Content-Type');
        var mimeType = contentType.match(/\s*([^;]+)\s*(;|$)/i)[1];
		switch(mimeType)
		{
			case 'application/json':break;
			case 'text/xml':
			case 'application/xml':
			case 'application/xhtml+xml':
			{		
				var xmlDom = this.responseXML;
				ajax.innerHTML = text(xmlDom.getElementsByTagName('xml')[0]);
			}break;
			case 'text/html':
			{				
				ajax.innerHTML = this.responseText;
			};break;
		}
	}
}
var submit = new Array();
for(var i=1;i<=4;i++){
	submit.push($('submit'+i));
}
for(var i=0;i<4;i++){
	addEventListener(submit[i],'click',listener);
}
var ajax = $('ajax');
</script>
</body>
</html>