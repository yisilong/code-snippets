<!-- MD5在线验证小工具 -->
<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8" />
	<meta name="keywords" content="Web标准化交流会,w3c交流会,web标准化联盟,前端，前端开发,w3ctech,HTML5,CSS3,JavaScript" />
	<meta name="description" content="w3ctech成立于2009年，为了帮助更多前端工程师成长，旗下拥有Web标准化交流会及系列专题活动。将定期与不定期与大家一起分享与交流前端技术。" />
	<title>MD5在线验证小工具</title>
</head>
<body>
<?php 
	if (isset($_FILES['file']))
	{
		if ($_FILES['file']['size'] > 1024*1024*10) // 大于10m
		{
			echo '文件超过大小限制10M';
		}
		else if ($_FILES['file']['error']) // 文件有错误
		{
			echo '文件上传出错';
		}
		else// if ($_FILES["file"]["tmp_name"])
		{
			$md5 = md5_file($_FILES['file']['tmp_name']);
			if ($md5) 
			{
				echo 'MD5:'.$md5;
			}
			else
			{
				echo 'MD5:计算出错';
			}
			$sha1 = sha1_file($_FILES['file']['tmp_name']);
			if ($sha1)
			{
				echo '<br />SHA1:'.$sha1;
			}
			else
			{
				echo '<br />SHA1:计算出错';
			}
		}
	}
	else
	{
?>
	<form method="post" action="index.php" enctype="multipart/form-data">
		<label for="file">选择文件:</label>
		<input type="file" name="file" />
		<input type="submit" name="submit" value="验证" />
	</form>
<?php }?>
</body>
</html>