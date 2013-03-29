<?php
//操作
function crumbHtml($path)
{
	$path_utf8=$GLOBALS['path_utf8'];;
	$crumbHtml='<div class="crumb" id="crumb">';
	$crumbHtml.='<span id="crumb-title">你的位置: </span> <span><a href="./">首页</a></span>';
	if($path!='') 
	{
		$paths=explode('/',$path_utf8);
		$tempPath='';
		for($i=0;$i<count($paths)-1;$i++)
		{
			$tempPath.=$paths[$i];
			$crumbHtml.='<span class="crumb-linker">&lt;&lt;</span><span><a href="'.$tempPath.'" class="ajaxPath">'.$paths[$i].'</a></span>';	
			$tempPath.='/';
		}
		$crumbHtml.='<span class="crumb-linker">&lt;&lt;</span><span>'.$paths[$i].'</span>';
	}
	$crumbHtml.='</div>';
	echo $crumbHtml;
}
?>
<?php
//得到路径列表
function operateHtml()
{?>
	<div class="operate" id="operate">
		<span>删除</span>
		<span>上传</span>
		<span>移动</span>
		<span>复制</span>
		<span>压缩</span>
		<span>解压</span>
		<span>打包</span>
		<span id="refresh"><a href="" title="刷新文件列表">刷新</a></span>
		<form method="get" id="searchform" action="?action=search">
			<input type="text" class="field" name="s" id="s" placeholder="搜索">
			<input type="submit" class="submit" name="submit" id="searchsubmit" value="搜索">
		</form>
	</div>
<?php } ?>
<?php 
//得到文件列表
function fileList($path,$start,$pageLimit)
{	
	$path_utf8=$GLOBALS['path_utf8'];
	$results=false;
	if($path=='')
	{
		$results = scandir('./',1);//降序
	}
	else
	{	
		$path.='/';
		$path_utf8.='/';
		$results = scandir($path,1);//降序	
	}
	if(!$results)
	{
		require_once('die.php');
		exit(0);
	}
	array_pop($results); //删除.
	array_pop($results); //删除..

	$return='<form name="listForm" id="listForm" method="post" onsubmit="checkInput()">';
	//$return.='<input name="path" type="hidden" id="path" value="'.$path_utf8.'" />';
	//标题栏
	$return.='<div class="file-title" id="file-title">';
	$return.='<span class="block file-name"><span class="checkbox"><input name="checkAll" type="checkbox" id="checkAll" /></span><span class="filename">名称</span></span>';
	$return.='<span class="block file-size">大小</span>';
	$return.='<span class="block file-time">时间</span>';
	$return.='<span class="block file-action">操作</span>';
	$return.='</div>';	
	//文件列表
	$folders='';
	$files='';
	$end=min($start+$pageLimit,count($results));	
	for($i=$start;$i<$end;$i++)
	{
		$stat=stat($path.$results[$i]);
		$file_utf8=iconv("gb2312","UTF-8",$results[$i]);
		if($stat)
		{
			if (is_dir($path.$results[$i])) //是目录
			{
				$folders.='<div class="file">';
				$folders.='<span class="block file-name"><span class="checkbox"><input name="file[]" type="checkbox" value="'.$path_utf8.$file_utf8.'" /></span><span class="filename typefolder"><a href="'.$path_utf8.$file_utf8.'" class="ajaxPath">'.$file_utf8.'</a></span></span>';
				$folders.='<span class="block file-size"></span>';
				$folders.='<span class="block file-time">'.date('Y/m/d h:i:s',$stat['mtime']).'</span>';
				$folders.='<span class="block file-action"><a href="?action=rename" onclick="return false">重命名</a></span>';
				$folders.='</div>';
			}
			else
			{
				$files.='<div class="file">';
				$files.='<span class="block file-name"><span class="checkbox"><input name="file[]" type="checkbox" value="'.$path_utf8.$file_utf8.'" /></span><span class="filename typefile"><a href="'.$path_utf8.$file_utf8.'" target="_blank">'.$file_utf8.'</a></span></span>';
				$size=sprintf("%u",$stat['size']);//防止大于2g时返回负数
				/*if($size<1024) //小于1K
				{
					$size.=' Bytes';
				}
				else*/if($size<1024*1024)//小于1M
				{
					$size=sprintf("%'#6.2f",$size/1024).' KB';
				}
				elseif($size<1024*1024*1024)//小于1G
				{
					$size=sprintf("%'#6.2f",$size/(1024*1024)).' MB';
				}
				else //大于等于1G
				{
					$size=sprintf("%'#6.2f",$size/(1024*1024*1024)).' GB';
				}
				$size=str_replace('#','&nbsp;',$size);
				$files.='<span class="block file-size">'.$size.'</span>';
				$files.='<span class="block file-time">'.date('Y/m/d h:i:s',$stat['mtime']).'</span>';
				$files.='<span class="block file-action"><a href="?action=rename" onclick="return false">重命名</a></span>';
				$files.='</div>';				
			}
		}		
	}
	$return.='<div class="file-list" id="file-list">'.$folders.$files.'</div>';
	$return.='</form>';
	echo $return;
}
?>