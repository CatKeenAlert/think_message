<?php 
class PublicAction extends Action{
	public function code()
	{
		 import('ORG.Util.Image');
   		 Image::buildImageVerify(4,1,'png',"$_GET[width]","$_GET[height]",'verify');
	}
 }
 ?>
