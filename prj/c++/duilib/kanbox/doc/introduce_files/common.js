String.prototype.trim = function() {
	return this.replace(/^\s+|\s+$/g,'');
}
$.extend(String.prototype,{
	format:function() {
	    var s = this,
        i = arguments.length;

	    while (i--) {
	    	var c=arguments[i];
	    	if(typeof(c)=='string'){
	    		c=c.replace(/\$/g,"$$$$");
	    	}
	        s = s.replace(new RegExp('\\{' + i + '\\}', 'gm'), c);
	    }
	    return s;
	},
	//是否为utf8中文
	isChineseUTF:function(){
		var re=/[\u4E00-\u9FA5]|[\uFE30-\uFFA0]/gi;
		if(re.test(this))
			return true;
		else
			return false;
	}
});

var kanbox=kanbox||function(){};
kanbox.debugable=false;
//打开调试环境
kanbox.opendebug=function(){
	kanbox.debugable=true;
}
kanbox.timeout = 10000;

$.extend(kanbox, {
	validateEmail: function(email) {
		var re = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
		if(re.test(email) == false) {
			return false;
		}
		return true;
	},
	showMessage: function(msg,timeout){
		var msgbox=$('#topmsg');
		msgbox.show();
		msgbox.jnotifyAddMessage({
			text: msg.replace(/\&/g,'&amp;'),
			permanent: false,
			showIcon: false,
			disappearTime:timeout
		});
		var isIe = $.browser.msie;
		var isIe6 = $.browser.msie && ('6.0' == $.browser.version);
		var isIe7 = $.browser.msie && ('7.0' == $.browser.version);
		if(isIe6||isIe7){
			msgbox.css({width:300});
		}
		msgbox.css({
			left:'50%',
			marginLeft:'-{0}px'.format(msgbox.outerWidth()/2)
		});
	}
});
