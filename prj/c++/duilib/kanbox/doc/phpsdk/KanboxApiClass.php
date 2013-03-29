<?php
/**
 * 本类需要php5.2以上版本，且需要支持curl库
 * 任何有关酷盘api的问题，可以到open.kanbox.com中的开发者论坛中反馈，谢谢您的支持
 */
require_once 'sdkconfig.inc.php';
require_once 'KanboxApi.php';
class KanboxApiClass extends KanboxApi {
	private $_config;
	private $ch;
	
	public function __construct(){
		global $kbsdkconfig;
		$this->_config=$kbsdkconfig;
	}
	
	private function _checkfunction($function=NULL){
		return method_exists(__CLASS__,$function);
	}
	
	private function _parse_response($response){
		list($header,$body)= explode("\r\n\r\n", $response, 2);
		preg_match("/error='.*'/i",$header,$header_index);
		if (empty($header_index)){
			$body=json_decode($body,true);
			return array('suc'=>TRUE,'data'=>$body);
		}else {
			$parse_header=explode("'", $header_index[0]);
			return array('suc'=>FALSE,'data'=>$parse_header[1]);
		}
	}
	
	public function call_function($access_token=NULL,$function=NULL,$param=array()){
		if (!$this->_checkfunction($function)){
			return FALSE;
		}
		header("Content-Type: text/html; charset=UTF-8");
		return call_user_func_array(array(__CLASS__,$function), array($access_token,$param));
	}
	
	private function _set_curl_init($access_token){
		$this->ch = curl_init();
		curl_setopt($this->ch, CURLOPT_HTTPHEADER, array("Authorization: Bearer ".$access_token));
		curl_setopt($this->ch, CURLOPT_HEADER, 1);
        curl_setopt($this->ch,CURLOPT_RETURNTRANSFER,true);
        curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER,$this->_config->sslverify);
	}
	
	protected function account_info($access_token=NULL){
		$this->_set_curl_init($access_token);
        curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'info');
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function files_list($access_token=NULL,$param=array('path'=>'')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'list'.$param['path']);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function get_down_url($access_token=NULL,$param=array('path'=>'')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'download'.$param['path']);
        curl_setopt($this->ch, CURLOPT_NOBODY, 1);
        $result = curl_exec($this->ch);
        $httpstat=curl_getinfo($this->ch,CURLINFO_HTTP_CODE);
        curl_close($this->ch);
        switch ($httpstat){
        	case 302:
        		return array('suc'=>TRUE,'data'=>trim(substr($result,strpos($result,'Location:')+10)));
        		break;
        	case 401:
        		return array('suc'=>FALSE,'data'=>'UNAUTHENTICATED');
        		break;
        	case 404:
        		return array('suc'=>FALSE,'data'=>'FILE_NOT_FOUND');
        		break;
        }
	}
	
	protected function upload_file($access_token=NULL,$param=array('path'=>'','destination_path'=>'')){
//		$contents=file_get_contents($param['path']);
//		$postopt=array('http'=>array(
//			'method'=>'POST',
//			'header'=>"Content-Type: application/x-www-form-urlencoded\r\n".
//				"Connection: keep-alive\r\n".
//				"Authorization: Bearer ".$access_token."\r\n",
//			'content'=>$contents
//		));
//		$postcontext=stream_context_create($postopt);
//		$fp = @fopen($this->_config->upload_url.'upload'.$param['destination_path'], 'rb', false, $postcontext);

//		if (! $fp) {
//			die ( "can not open server!" );
//		}
//			
//		$response = @stream_get_contents ( $fp );
//		if ($response === false) {
//			die ( "can not get message form server!" );
//		
//		//throw new Exception("Problem reading data from {$url}, {$php_errormsg}");
//		}
//		return $response;
		
		$this->_set_curl_init($access_token);
        curl_setopt($this->ch,CURLOPT_URL,$this->_config->upload_url.'upload'.$param['destination_path']);
		curl_setopt($this->ch, CURLOPT_POSTFIELDS,file_get_contents($param['path']));
        $result = curl_exec($this->ch);
        curl_close($this->ch);
		$result=explode(' ', $result);
		switch ($result[1]){
			case '200':$result='ok';break;
			case '401':$result='Unauthorized';break;
			default:$result='SERVER_ERROR';break;
		}
        return $result;
	}
	
	protected function delete_file($access_token=NULL,$param=array('path'=>'')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'delete'.$param['path']);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function move_file($access_token=NULL,$param=array('path'=>'','destination_path'=>'')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'move'.$param['path'].'?destination_path='.$param['destination_path']);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function copy_file($access_token=NULL,$param=array('path'=>'','destination_path'=>'')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'copy'.$param['path'].'?destination_path='.$param['destination_path']);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function create_folder($access_token=NULL,$param=array('path'=>'')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'create_folder'.$param['path']);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function thumbnail($access_token=NULL,$param=array('path'=>'','size'=>'small')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'thumbnail'.$param['path'].'?size='.$param['size']);
        $result = curl_exec($this->ch);
		$httpstat=curl_getinfo($this->ch,CURLINFO_HTTP_CODE);
        curl_close($this->ch);
        switch ($httpstat){
        	case 302:
        		return array('suc'=>TRUE,'data'=>trim(substr($result,strpos($result,'Location:')+10)));
        		break;
        	case 401:
        		return array('suc'=>FALSE,'data'=>'UNAUTHENTICATED');
        		break;
        	case 404:
        		return array('suc'=>FALSE,'data'=>'FILE_NOT_FOUND');
        		break;
        }
	}
	
	protected function create_share($access_token=NULL,$param=array('path'=>'','users'=>array())){
		$this->_set_curl_init($access_token);
		$requesturi=json_encode($param['users']);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'share'.$param['path']);
        curl_setopt($this->ch, CURLOPT_POSTFIELDS,$requesturi);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function pending_shares($access_token=NULL,$param=array('process'=>array('path'=>'','user'=>'','accept'=>FALSE))){
		$this->_set_curl_init($access_token);
        curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'pendingshares');
        if (!is_null($param)){
        	$requesturi=json_encode($param['process']);
        	curl_setopt($this->ch, CURLOPT_POSTFIELDS,$requesturi);
        }
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
	
	protected function check_owner($access_token=NULL,$param=array('path'=>'')){
		$this->_set_curl_init($access_token);
        @curl_setopt($this->ch,CURLOPT_URL,$this->_config->request_url.'checkowner'.$param['path']);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return $this->_parse_response($result);
	}
}
?>