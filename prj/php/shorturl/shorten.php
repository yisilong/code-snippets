<?php
isset($_GET['dest']) and ($_GET['dest'] == 'sina' or $_GET['dest'] == 'baidu') or die('error');
isset($_GET['url']) or die('error');
$dest = $_GET['dest'];

$api_sina = 'https://api.weibo.com/2/short_url/shorten.json?source=2006100199'; // get方式
$api_baidu = 'http://dwz.cn/create.php'; // post方式
$query_str = $_SERVER['QUERY_STRING'];
// 分解出url  dest=baidu&url=http://www.baidu.com/&url=http://www.163.com/
$urls = array();
$begin = strpos($query_str,'url');
while ($begin !== false){
	$begin += strlen('url=');
	$end = strpos($query_str,'&',$begin);
	if ($end === false){
		$urls[] = substr($query_str,$begin);
		break;
	}
	else{
		$urls[] = substr($query_str,$begin,$end-$begin);
	}
	$begin = strpos($query_str,'url',$end);
}
// 获取短链
$tinyurls = array();
if ($dest == 'sina'){
	$ncount = 0;
	$str_request = '';
	foreach($urls as $url){
		$ncount++;
		$str_request .= '&url_long='.urlencode($url);
		if ($ncount == 20){
			parse_sina($tinyurls,$api_sina.$str_request);
			$str_request = '';
			$ncount = 0;			
		}
	}
	if ($ncount != 0){
		parse_sina($tinyurls,$api_sina.$str_request);
	}
}
else{
	$ch=curl_init();
	curl_setopt($ch,CURLOPT_URL,$api_baidu);
	curl_setopt($ch,CURLOPT_POST,true);
	curl_setopt($ch,CURLOPT_RETURNTRANSFER,true);
	foreach($urls as $url){
		curl_setopt($ch,CURLOPT_POSTFIELDS,'url='.urlencode($url));
		$strRes=curl_exec($ch);
		$arrResponse=json_decode($strRes,true);
		if($arrResponse['status'] == 0)
		{
			$tinyurls[$url] = $arrResponse['tinyurl'];
		}
	}
	curl_close($ch);
}
print_r($tinyurls);
?>

<?php
function parse_sina(&$tinyurls,$str){
	$ch=curl_init();
	curl_setopt($ch,CURLOPT_URL,$str);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);  
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
	curl_setopt($ch,CURLOPT_RETURNTRANSFER,true);
	$strRes=curl_exec($ch);
	curl_close($ch);
	$strRes='{"urls":[{"result":true,"url_short":"http://t.cn/h5mwx","url_long":"http://www.baidu.com","type":25},{"result":true,"url_short":"http://t.cn/h5gEb","url_long":"http://www.126.com","type":0}]}';
	$arrResponse=json_decode($strRes,true);
	if (isset($arrResponse['urls']))
	{
		foreach($arrResponse['urls'] as $result){
			$tinyurls[$result['url_long']] = $result['url_short'];
		}
	}
}
?>