<?php 
class CommonAction extends Action{
	public function _initialize()
	{
		if(!(isset($_SESSION['username']) && $_SESSION['username'])){
			$this->redirect("Login/login");
		}
	}
}
?>
