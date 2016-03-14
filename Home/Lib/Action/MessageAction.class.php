<?php 
class MessageAction extends Action{
	public function doMessage()
	{
		$message = D('Message'); // 实例化User对象
		$message->create(); // 创建数据对象
		import('ORG.Net.UploadFile');
		$upload = new UploadFile();// 实例化上传类
		//$upload->maxSize  = 3145728 ;// 设置附件上传大小
		//$upload->allowExts  = array('jpg', 'gif', 'png', 'jpeg');// 设置附件上传类型
		$upload->savePath =  './Public/Uploads/';// 设置附件上传目录
		if(!$upload->upload()) {// 上传错误提示错误信息
			$this->error($upload->getErrorMsg());
		}else{// 上传成功 获取上传文件信息
			$info =  $upload->getUploadFileInfo();
		}
		// 保存表单数据 包括附件数据
		$message->filename = $info[0]['savename'];
		// $message->time = time();
		// $message->uid = $_SESSION['id'];
		// var_dump($message);
		// exit;
		$lastId = $message->add(); // 写入用户数据到数据库
		if($lastId){
			$this->success('留言成功');			
		}else{
			$this->error('留言失败');
		}
	}
}

 ?>