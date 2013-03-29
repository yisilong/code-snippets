<?php 
//验证身份
if(!isset($_SESSION['userName']))
{
	if(isset($_POST['userName']) && isset($_POST['passWord']))
	{
		if(in_array(array($_POST['userName']=>$_POST['passWord']),$tokens,true))
		{
			$_SESSION['userName'] = $_POST['userName'];
		}
		else
		{
			$login='failed';
			require_once('login.php');
			exit(0);
		}
	}
	else
	{
		$login='login';
		require_once('login.php');
		exit(0);		
	}
}
?>