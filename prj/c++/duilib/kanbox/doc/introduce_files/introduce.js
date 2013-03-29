// JavaScript Document
$(function(){
	$(".menu>.nav").click(function(){
		$('.menu>.nav').find('a').hide();
		$(this).find('a').show();
		//$(this).children("a").show()
		//.end().siblings().children("a").hide();
	});

	
	var infofloater=new kanbox.infofloater($('#leftnavarea'));
	infofloater.init();
	
});