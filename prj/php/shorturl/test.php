<?php
$data_string = <<<fuck
'<?xml version="1.0" encoding="UTF-8"?>
<xml>
	<token></token>
	<urls>
		<url>http://www.baidu.com/</url>
		<url>http://www.ixxoo.me/</url>
		<url>https://www.google.com/</url>
	</urls>
</xml>'
fuck;
$URL_Info["port"]=80;
// building POST-request:
$request ="POST ".'/shorturl.php'." HTTP/1.1\n";
$request.="Host: ".'127.0.0.1'."\n";
$request.="Content-type: text/xml\n";
$request.="Content-length: ".strlen($data_string)."\n";
$request.="Connection: close\n";
$request.="\n";
$request.=$data_string."\n";
$fp = fsockopen('127.0.0.1',$URL_Info["port"]);
fputs($fp, $request);
$result='';
while(!feof($fp)) {
    $result .= fgets($fp, 128);
}
fclose($fp);
?>