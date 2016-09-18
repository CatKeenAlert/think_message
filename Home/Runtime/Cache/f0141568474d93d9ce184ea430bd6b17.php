<?php if (!defined('THINK_PATH')) exit();?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Document</title>
	<link rel="stylesheet" type="text/css" href="__PUBLIC__/Css/login.css" />
	<script type="text/javascript" src="__PUBLIC__/Js/jquery-1.9.0.main.js"></script>
	<!-- <script type="text/javascript" src="__PUBLIC__/Js/login.js"></script> -->
	<script type="text/javascript">
		$(function() {
			$("img[id='img_login']").click(function(){
				$("form[name='form']").submit();
			});
			$("img[id='img_register']").click(function(){
				window.location="__APP__/Register/index";
			});
		});
	</script>
</head>
<body>
	<form action="__URL__/do_login" method="post" name="form">
		<table>
			<tr>
			<td>用户名<input type="text" name="username"></td>
			</tr>
			<tr>
			<td>密&nbsp;码<input type="password" name="password"></td>
			</tr>
			<tr>
			<td>验证码<input type="text" name="code"></td>
			<!-- <td>
				<img src='__APP__/Public/code?width=2px&height=2px' onclick='this.src=this.src+'?'+Math.random()'>
			</td> -->
			<td><img src="__APP__/Public/code?width=30&height=20" onclick="this.src=this.src+'?'+Math.random()"></td> 
			</tr>
			<tr>
				<td>
				<img id='img_login' class="submit" src='__PUBLIC__/Images/login.gif' onclick="login()">
				<img id='img_register' class="submit" src="__PUBLIC__/Images/register.gif" onclick="register">
				</td>
			</tr>
		</table>
	</form>
</body>
</html>