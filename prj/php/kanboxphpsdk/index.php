<?php
require_once 'kbAuthClass.php';
$kbauth=new kbAuthClass();
$redirect_url='http://'.$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF']).'/demo.php';
header('location: '.$kbauth->getAuthorizeURL($redirect_url));
?>