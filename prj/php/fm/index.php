<?php
session_start();
require_once('config.inc.php');
require_once('checkToken.php'); //验证身份
//验证路径是否
$path_utf8='';
isset($_POST['path']) and $path_utf8=$_POST['path'];
$path=iconv("UTF-8","gb2312",$path_utf8); //转gbk
if($path!='' && !is_dir($path))
{
	require_once('die.php');
	exit(0);
}
$start = 0;
if(isset($_POST['start'])) $start = $_POST['start'];
require_once('functions.php');
?>
<!DOCTYPE HTML>
<html>
<?php require_once('header.php');?>
<body>
<div class="file-container" id="file-container">
	<?php operateHtml($path);?>
	<?php crumbHtml($path);?>
	<?php fileList($path,$start,$pageLimit);?> 
</div>
<!-- <input type="file" id="file_upload" name="file_upload" /> -->
<script type="text/javascript" src="js/fm.js"></script>
</body>
</html>