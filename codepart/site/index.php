<?php
	/********************************************************
	* 列表页,分类页
	********************************************************/
	$type=isset($_GET['type']) ? urldecode($_GET['type']):''; 
	$page_num=isset($_GET['page']) ? $_GET['page']:1;
	$page_size = 100;
	$strsql = 'SELECT count(id) FROM posts';
	$strsql1 = 'SELECT id,type,title FROM posts LIMIT '.($page_num-1)*$page_size.','.$page_size;
	if ($type != '')
	{
		$strsql .= " WHERE type='".$type."';";
		$strsql1 .= " WHERE type='".$type."';";
	}
?>
<!DOCTYPE html>
<html>
<head>
	<title>xxoo影视|电影下载-电影BT种子下载|720P高清电影 1080P高清电影下载|最新最热电影下载</title>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
	<meta name="keywords" content="高清电影下载,最新电影,电影下载,bt下载,最热门电影下载,bt种子,电影BT下载,720P高清电影,1080P高清电影" />
	<meta name="description" content="xxoo影视为您提供最新最全的BT种子迅雷电影下载|旋风电影下载|快车电影下载|在线观看高清电影|高清电影下载，电影BT下载，迅雷下载，旋风下载。各种经典电影推荐，动作、悬疑、爱情、喜剧、惊悚、老电影、奥斯卡最佳电影下载等等。 " />
	<link href="style.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="content">
		<ul id="posts">
		<?php
			$db = new SQLite3('data.db');
			$result = $db->query($strsql);
			$res = $result->fetchArray();
			$post_count = $res[0];
			$total_pages = intval(($post_count + $page_size -1)/$page_size);
			$result = $db->query($strsql1);
			$lis = '';
			while ($res = $result->fetchArray())
			{
				$id = $res['id'];
				$type = iconv("GBK","utf-8",$res['type']);
				$title = iconv("GBK","utf-8",$res['title']);
				$lis .= '<li>';
				$lis .= '<a href="type-'.urlencode($type).'.html" class="type" target="_blank"> ['.$type.']</a>';
				$lis .= '<a href="post-'.$id.'.html" class="title" target="_blank">'.$title.'</a>';
				$lis .= '</li>';
			}
			echo $lis;
		?>
		</ul>
		<ul id="pages">
			<?php
				/*$page_begin = $page_num - 5;
				$page_end = $page_num + 5;
				$str_dots_begin = $page_begin > 1 ? '<li>...</li>':'';
				if ($page_begin < 1)
				{
					$page_begin = 1;
				}
				$str_dots_end = $page_end > $total_pages ? '':'<li>...</li>';
				if ($page_end > $total_pages)
				{
					$page_end = $total_pages;
				}
				// 修正
				if ($page_num < 6)
				{
					$page_num = 1;
					$page_num = 11;
				}
				else if($total_pages-$page_num < 6)
				{
					$page_end = $total_pages;
					$page_begin = $page_end - 10;
				}*/
				$pages = '<li><a href="list-'.'1'.'.html'.'">'."首页".'</a></li>';
				//$pages .= $str_dots_begin;
				for($i=1;$i<=$total_pages;$i++)
				{
					$pages .= '<li><a href="list-'.$i.'.html'.'">'.$i.'</a></li>';
				}
				//$pages .= $str_dots_end;
				$pages .= '<li><a href="list-'.$total_pages.'.html'.'">'."末页".'</a></li>';
				echo $pages;
				$db->close();
			?>
		</ul>
	</div>
</body>
</html>