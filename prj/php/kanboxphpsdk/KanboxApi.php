<?php
/**
 * 本类需要php5.2以上版本，且需要支持curl库
 * 任何有关酷盘api的问题，可以到open.kanbox.com中的开发者论坛中反馈，谢谢您的支持
 */
abstract class KanboxApi {
	
	/**
	 * get user's account info
	 * @param string $access_token
	 */
	abstract protected function account_info($access_token=NULL);
	
	/**
	 * get the files list by path that you request 
	 * @param string $access_token
	 * @param array $param
	 */
	abstract protected function files_list($access_token=NULL,$param=array('path'=>''));
	
	/**
	 * get download url of the file you want to download
	 * @param string $access_token
	 * @param array $param
	 */
	abstract protected function get_down_url($access_token=NULL,$param=array('path'=>''));
	
	/**
	 * upload a file ,need to provide a file path and the destination path that you want to restore in kanbox
	 * @param string $access_token
	 * @param array $param
	 */
	abstract protected function upload_file($access_token=NULL,$param=array('path'=>'','destination_path'=>''));
	
	/**
	 * delete a file or folder by path that you request
	 * @param string $access_token
	 * @param array $param
	 */
	abstract protected function delete_file($access_token=NULL,$param=array('path'=>''));
	
	/**
	 * move a file or folder by path that you request,also need the destination path that you want to restore in kanbox
	 * @param unknown_type $access_token
	 * @param unknown_type $param
	 */
	abstract protected function move_file($access_token=NULL,$param=array('path'=>'','destination_path'=>''));
	
	/**
	 * copy a file or folder by path that you request,also need the destination path that you want to restore in kanbox
	 * @param unknown_type $access_token
	 * @param unknown_type $param
	 */
	abstract protected function copy_file($access_token=NULL,$param=array('path'=>'','destination_path'=>''));
	
	/**
	 * create a new folder ,need to provide a path
	 * @param unknown_type $access_token
	 * @param unknown_type $param
	 */
	abstract protected function create_folder($access_token=NULL,$param=array('path'=>''));
	
	/**
	 * get thumbnail of a file that must be image type,the size can be set as small , medium , large
	 * @param string $access_token
	 * @param array $param
	 */
	abstract protected function thumbnail($access_token=NULL,$param=array('path'=>'','size'=>'small'));
	
	/**
	 * share a folder to other users,need to provide a folder path and users list that you want to share
	 * @param string $access_token
	 * @param array $param
	 */
	abstract protected function create_share($access_token=NULL,$param=array('path'=>'','users'=>array()));
	
	/**
	 * get the pendings of share folder if set the param NULL,or process a pending with param not NULL
	 * @param string $access_token
	 * @param NULL or array $param
	 */
	abstract protected function pending_shares($access_token=NULL,$param=array('process'=>array('path'=>'','user'=>'','accept'=>FALSE)));
	
	/**
	 * get a folder info if it is a share folder
	 * @param string $access_token
	 * @param array $param
	 */
	abstract protected function check_owner($access_token=NULL,$param=array('path'=>''));
}
?>
