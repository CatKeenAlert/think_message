<?php 
class MessageModel extends RelationModel{
	protected $_auto = array ( 
   		array('time','time',1,'function') , // 对time字段在新增的时候使md5函数处理
   	    array('uid','getId',1,'callback'), // 对uid字段在新增的时候回调getName方法
	);
	protected function getId()
	{
		return $_SESSION['id'];
	}
	protected $_link = array(
	   'user'=> array(  
	        'mapping_type'=>BELONGS_TO,
	        'class_name'=>'User',
	        'foreign_key'=>'uid',
	        'mapping_name'=>'user',
	        'as_fields'=>'username:uname',
	   ),
	);
}
 ?>