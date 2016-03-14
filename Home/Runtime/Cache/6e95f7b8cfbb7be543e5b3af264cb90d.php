<?php if (!defined('THINK_PATH')) exit();?><!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Document</title>
</head>
<body>
	<h1>ThinkPHP留言板系统</h1>
	<p>欢迎您,<?php echo ($_SESSION['username']); ?>
	<a href="__APP__/Login/login_off" target="_top" style="text-decoration:none;color:orange">退出</a>
	</p>
</body>
</html>