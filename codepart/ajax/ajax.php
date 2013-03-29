<?php
	if(isset($_POST['POST']))
	{
		if($_POST['POST'] == 'XML')
		{
			header('Content-type: text/xml');
			echo '<?xml version="1.0" ?><xml>post-xml</xml>';
		}
		else echo 'post-text';
	}
	elseif(isset($_GET['GET']))
	{
		if($_GET['GET'] == 'XML')
		{
			header('Content-type: text/xml');
			echo '<?xml version="1.0" ?><xml>get-xml</xml>';
		}
		else echo 'get-text';
	}
?>