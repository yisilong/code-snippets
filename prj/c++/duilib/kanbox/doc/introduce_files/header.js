$(function(){
	
	//页面头导航按钮
	var headerbuttons=function(){
		switch (location.pathname.substr(1,3)){
		case '':$('.api_index').addClass('nav_hover');break;
		case 'ind':$('.api_index').addClass('nav_hover');break;
		case 'int':$('.api_introduce').addClass('nav_hover');break;
		case 'app':$('.api_app').addClass('nav_hover');break;
		case 'bbs':$('.api_bbs').addClass('nav_hover');break;
		}
	};
	headerbuttons();

	
	//用户名菜单
	if($('.list_notice-box').size()==0){
		$('.user_f_div').css('top',$('.header_main').height());
	}
	$('.user_f_div li').click(function(){
		window.location=$(this).find('a').attr('href');
	});
	$('.user_f_div li').hover(function(){
		$(this).addClass('user_f_div_hover');
	},function(){
		$(this).removeClass('user_f_div_hover');
	});
	
	
	$('#user_op_menu').parent().click(function(e){
		if(!kanbox.menushow){
			$(this).next('div').show();
			kanbox.menushow=true;
		}else{
			$(this).next('div').hide();
			kanbox.menushow=false;
		}
		e.stopPropagation();
		return false;
	});
	$(document).click(function(e){
		$('#user_op_menu').parent().next('div').hide();
		kanbox.menushow=false;
		//$('#user_op_menu').removeClass('user_name_a1');
	});
});