<?php
require_once 'sdkconfig.inc.php';
class kbAuthClass  {
	private $_config;
	private $ch;

	public function __construct(){
		global $kbsdkconfig;
		$this->_config=$kbsdkconfig;
	}
	
	function getAuthorizeURL($redirect_uri=NULL){
		return 'https://auth.kanbox.com/0/auth?response_type=code&client_id='.$this->_config->client_id.'&platform=web&redirect_uri='.$redirect_uri.'&user_language=ZH';
	}
	
	function getTokenByAuthcode($authcode=NULL,$redirect_uri=NULL){
		$this->ch = curl_init();
		$requesturi='grant_type=authorization_code&client_id='.$this->_config->client_id.'&client_secret='.$this->_config->client_secret.'&code='.$authcode.'&redirect_uri='.$redirect_uri;
		curl_setopt($this->ch,CURLOPT_URL,'https://auth.kanbox.com/0/token');
        curl_setopt($this->ch,CURLOPT_POSTFIELDS,$requesturi);
        curl_setopt($this->ch,CURLOPT_RETURNTRANSFER,true);
        curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER,$this->_config->sslverify);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return json_decode($result,true);
	}
	
	function refreshNewAccesstoken($refreshtoken){
		$this->ch = curl_init();
		$requesturi='grant_type=refresh_token&client_id='.$this->_config->client_id.'&client_secret='.$this->_config->client_secret.'&refresh_token='.$refreshtoken;
		curl_setopt($this->ch,CURLOPT_URL,'https://auth.kanbox.com/0/token');
        curl_setopt($this->ch,CURLOPT_POSTFIELDS,$requesturi);
        curl_setopt($this->ch,CURLOPT_RETURNTRANSFER,true);
        curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER,$this->_config->sslverify);
        $result = curl_exec($this->ch);
        curl_close($this->ch);
        return json_decode($result,true);
	}
}
?>