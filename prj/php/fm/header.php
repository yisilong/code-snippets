<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>文件管理系统</title>
<link href="css/style.css" rel="stylesheet" type="text/css" />
<link href="uploadify/uploadify.css" type="text/css" rel="stylesheet" />
<script type="text/javascript" src="uploadify/jquery.min.js"></script>
<script type="text/javascript" src="uploadify/swfobject.js"></script>
<script type="text/javascript" src="uploadify/uploadify.min.js"></script>
<script type="text/javascript" src="js/functions.js"></script>
<script type="text/javascript">
$(document).ready(function() {
  $('#file_upload').uploadify({
    'uploader'  : './uploadify/uploadify.swf',
    'script'    : './uploadify/uploadify.php',
	//'checkScript' : './uploadify/check.php',
	'cancelImg' : './uploadify/cancel.png',
    'folder'    : './',
    'auto'      : true,
	'sizeLimit'   : 102400000,
	'multi'       : true,
	'displayData' : 'speed'
  });
  $(".file:odd").addClass("odd");
});
</script>
</head>