<!DOCTYPE html>
<html> 
<head> 
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /> 
	<title><?php echo $siteName.' - 登陆';?></title> 
	<link rel='stylesheet' href='css/login.css' type='text/css' /> 
</head> 
<body> 
<div id="login">
	<h1 id="logo"><a href="./" title="<?php echo $siteName;?>">在云端</a></h1>
	<?php if($login=='failed') { ?>
	<div id="login-error">	
		<strong>错误</strong>：用户名或密码错误<br />
	</div>
	<?php };?>
	<form name="loginform" id="loginform" action="./" method="post">
		<p> 
			<label for="userName">用户名<br /> 
			<input type="text" name="userName" id="userName" autofocus="true" class="input" value="" size="20" tabindex="1" /></label> 
		</p> 
		<p> 
			<label for="passWord">密码<br /> 
			<input type="password" name="passWord" id="passWord" class="input" value="" size="20" tabindex="2" /></label> 
		</p> 
		<p> 
			<input type="reset" name="reset" id="reset" value="重置" tabindex="3" />
			<input type="submit" name="submit" id="submit" value="登录" tabindex="4" />
		</p> 
</form> 
</div> 
<div class="clear"></div> 
</body>
</html> 