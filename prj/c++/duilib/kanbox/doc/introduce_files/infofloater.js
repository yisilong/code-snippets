var kanbox=kanbox||{};
//用途:信息浮动区管理
//example:
//var infofloater=new kanbox.infofloater($('#infoarea'));
//infofloater.init();

kanbox.infofloater=function(element) {
	this.element_=element;
	this.originalStyles_ = {};
};
$.extend(kanbox.infofloater,{
	STORED_STYLE_PROPS_ : ['position', 'top', 'left', 'width', 'cssFloat'],
	PLACEHOLDER_STYLE_PROPS_ : ['position', 'top', 'left', 'display', 'cssFloat',
	                            'marginTop', 'marginLeft', 'marginRight', 'marginBottom'],
	CSS_CLASS_ : 'infofloater',
	decorate : function(element){
		$(element).addClass(kanbox.infofloater.CSS_CLASS_);
	}
});
$.extend(kanbox.infofloater.prototype,{
	scrollingEnabled_ : true,
	floating_ : false,
	init : function(){
		if (!this.placeholder_) {
			this.placeholder_ = $('<div style="visibility:hidden"></div>');
		}
		this.originalOffset_ = $(this.element_).offset().top;
		this.originalOffsetLeft_ = $(this.element_).offset().left;
		this.setScrollingEnabled(this.scrollingEnabled_);
		var that=this;
		/*if(!this.originalOffset_){
			setTimeout(function(){that.originalOffset_ = $(that.element_).offset().top;},500);
		}*/
		$(window).scroll(function(){
			that.update_();
		}).resize(function(){
			that.handleResize_();
		});
	},
	setScrollingEnabled : function(enable) {
		this.scrollingEnabled_ = enable;
		if (enable) {
			this.applyIeBgHack_();
			this.update_();
		} else {
			this.stopFloating_();
		}
	},
	isScrollingEnabled : function() {
		return this.scrollingEnabled_;
	},
	isFloating : function() {
		return this.floating_;
	},
	getElement : function(){
		return this.element_;
	},
	update_ : function(opt_e) {
		if (this.scrollingEnabled_) {
			var currentScrollTop=$(document).scrollTop();
			if (currentScrollTop > this.originalOffset_) {
				this.startFloating_();
			} else {
				this.stopFloating_();
			}
		}
	},
	startFloating_ : function() {
		var elem = this.getElement();
		var scrollLeft_ = $(document).scrollLeft();
		var floatis = this.floating_;
		if (this.floating_) {
			if (!this.needsIePositionHack_()) {
				$(elem).css({
					'left': this.originalOffsetLeft_-scrollLeft_
				});
			}else {
				$(elem)[0].style.position = 'absolute';
				var elemlength = $(window).height() / 2;
				var scrolllength = document.compatMode=="CSS1Compat"? document.documentElement.scrollTop:document.body.scrollTop;
				$(elem)[0].style.setExpression('top',scrolllength - $(elem).height() +  elemlength );
			}
			return;
		}
		// Read properties of element before modifying it.
		var originalLeft_ = $(elem).offset().left;
		var originalWidth_ = $(elem).width();

		this.originalStyles_ = {};
		var that=this;
		$.each(kanbox.infofloater.STORED_STYLE_PROPS_,function(i,e){
			that.originalStyles_[e] = $(elem)[0].style[e];
		});
		$.each(kanbox.infofloater.PLACEHOLDER_STYLE_PROPS_,function(i,e){
			that.placeholder_.css(e,$(elem).css(e));
		});

		this.placeholder_.width($(elem).width());
		this.placeholder_.height($(elem).height());
		$(elem).css({
			'width': originalWidth_,
			'cssFloat': 'none'
		});
		//$('body').append(this.placeholder_);
		
		if (this.needsIePositionHack_()) {
			$(elem)[0].style.position = 'absolute';
			var righth = $(window).height()-$(elem).height();
			if( righth > 0 ){
			$(elem)[0].style.setExpression('top',
					'document.compatMode=="CSS1Compat"?' +
					'document.documentElement.scrollTop:document.body.scrollTop');
			}else {
				$(elem)[0].style.setExpression('top','document.documentElement.scrollTop'+righth);			
			}
		} else {
			var p=$(elem).parent();
			var righth = $(window).height()-$(elem).height();
			var righttop = righth>0 ? 0:righth;
			this.placeholder_.appendTo(p);
			$(elem).detach().appendTo('body');
			$(elem).css({
				'left': this.originalOffsetLeft_-scrollLeft_,
				position : 'fixed',
				top : righttop
			});
		}
		//$(elem).css({top : 0,left:0});
		//$(this.placeholder_).css({visibility:'hidden'});
		this.floating_ = true;
	},
	stopFloating_ : function() {
		if (this.floating_) {
		    var elem = this.getElement();

		    for (var prop in this.originalStyles_) {
		    	$(elem).css(prop,this.originalStyles_[prop]);
		    }

		    if (this.needsIePositionHack_()) {
		    	$(elem)[0].style.removeExpression('top');
		    }
		    if (!this.needsIePositionHack_()) {
			    var p=this.placeholder_.parent();
			    $(elem).detach();
			    this.placeholder_.detach();
			    $(elem).appendTo(p);
		    }
		    this.floating_ = false;
		}
	},
	handleResize_ : function() {
		this.stopFloating_();
		//this.originalOffset_ = $(this.getElement()).offset().top;
		this.originalOffsetLeft_ = $(this.getElement()).offset().left;
		this.update_();
	},
	needsIePositionHack_ : function() {
		var isIe = $.browser.msie;
		var isIe6 = $.browser.msie && ('6.0' == $.browser.version);
		var isIe7 = $.browser.msie && ('7.0' == $.browser.version);
		return isIe6;
	},
	applyIeBgHack_ : function() {
		if (this.needsIePositionHack_()) {
			topLevelElement=$('body')[0];
			if (topLevelElement.currentStyle.backgroundImage == 'none') {
			      // Using an https URL if the current windowbp is https avoids an IE
			      // "This page contains a mix of secure and nonsecure items" warning.
			    topLevelElement.style.backgroundImage =
			          window.location.protocol == 'https:' ?
			              'url(https:///)' : 'url(about:blank)';
			    topLevelElement.style.backgroundAttachment = 'fixed';
			}
		}
	}
});