<?php
require_once 'kbAuthClass.php';
require_once 'KanboxApiClass.php';

$redirect_url='http://'.$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'];
session_start();

unset($_SESSION);
//$_SESSION['kb_access_token']='1324620023';
if(isset($_SESSION['kb_access_token']) && $_SESSION['kb_access_token']){
	var_dump($_SESSION);
}
elseif (isset($_SESSION['kb_refresh_token']) && $_SESSION['kb_refresh_token'])
{
	$kbauth=new kbAuthClass();
	$refreshtoken=$kbauth->refreshNewAccesstoken($_SESSION['kb_refresh_token']);
	$_SESSION['kb_refresh_token']=$refreshtoken['refresh_token'];
	$_SESSION['kb_access_token']=$refreshtoken['access_token'];
	var_dump($_SESSION);
	echo '<br>';
}
else 
{
	$kbauth=new kbAuthClass();
	if (isset($_GET['code'])&&$_GET['code']){
		$token=$kbauth->getTokenByAuthcode($_GET['code'],$redirect_url);
		$_SESSION['kb_refresh_token']=$token['refresh_token'];
		$_SESSION['kb_access_token']=$token['access_token'];
	}
	else
	{
		header('location: '.$kbauth->getAuthorizeURL($redirect_url));
	}
}

$apiinstance=new KanboxApiClass();

//获取账户信息，包括酷盘空间大小及使用情况
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'account_info'));
//获取由path指定的路径下，文件列表信息，path类型为string，为空时，默认访问根目录
$return = $apiinstance->call_function($_SESSION['kb_access_token'],'files_list',array('path'=>'/other'));
$string="";
if($return['data']['status'])
{
	$filelist = $return['data']['contents'];
	foreach($filelist as $file)
	{
		foreach($file as $key => $value)
		{
			$string=$string.$key.':'.$value.' ';
		}
		$string=$string.'<br />';
	}
	echo $string;
}
//获取由path指定的文件，对应的下载地址，path类型为string
$return = $apiinstance->call_function($_SESSION['kb_access_token'],'get_down_url',array('path'=>'/other/avatar.zip'));
//echo '<br /><img src="'.$return['data'].'" />';
echo $return['data'];
//上传文件，path类型为string，destination_path为将此文件上传到酷盘后对应的路径及文件名，destination_path类型为string
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'upload_file',array('path'=>'http://192.168.5.43:81/phpsdk/ssd.txt','destination_path'=>'/ssd.txt')));
//删除由path指定的路径下的文件或文件夹，path类型为string
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'delete_file',array('path'=>'/ddd.pdf')));
//移动由path指定的路径下的文件或文件夹，到destination_path指定的路径，path及destination_path的类型均为string，可用于重命名，路径不变，替换文件名即可
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'move_file',array('path'=>'/ccc.pdf','destination_path'=>'/ddd.pdf')));
//复制由path指定的路径下的文件或文件夹，到destination_path指定的路径，path及destination_path的类型均为string
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'copy_file',array('path'=>'/ddd.pdf','destination_path'=>'/ccc.pdf')));
//创建文件夹，path为指定新文件夹，在酷盘中的路径
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'create_folder',array('path'=>'/酷盘apitest')));
//获取由path指定的图片文件，对应的缩略图下载地址，path类型为string，size为指定缩略图质量，可选项有small , medium , large，默认medium
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'thumbnail',array('path'=>'/示例1.jpg','size'=>'large')));
//创建文件夹共享，path为共享文件夹的路径，user为邀请加入共享的用户列表，path类型为string，users类型为string array
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'create_share',array('path'=>'/ddd','users'=>array('c@c.com'))));
//获取收到的文件夹共享邀请，不需要额外提供参数
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'pending_shares',NULL));
//处理一个文件夹共享邀请，path为邀请的文件夹路径，user为邀请者，accept为处理结果，false表示拒绝，true表示接受，path和user类型为string，accept为boolen
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'pending_shares',array('process'=>array('path'=>'/ddd','user'=>'b@b.com','accept'=>FALSE))));
//查看已存在的共享文件夹，创建者是否为自己，path为共享文件夹路径，类型为string
//var_dump($apiinstance->call_function($_SESSION['kb_access_token'],'check_owner',array('path'=>'/ddd')));

?>