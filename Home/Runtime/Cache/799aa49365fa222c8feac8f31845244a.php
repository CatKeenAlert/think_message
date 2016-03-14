<?php if (!defined('THINK_PATH')) exit();?><!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Document</title>
</head>
<body>
	<?php if(is_array($list)): foreach($list as $key=>$vo): ?><table>
			<tr>
				<td>留言标题:<?php echo ($vo["title"]); ?></td>
			</tr>
			<tr>
				<td>留言内容:<?php echo ($vo["content"]); ?></td>
			</tr>
			<tr>
				<td>留言时间:<?php echo (date("Y m d H:i:s",$vo["time"])); ?></td>
			</tr>
			<tr>
				<td>留言人:<?php echo ($vo["uname"]); ?></td>
			</tr>
			<tr>
				<td>附件名:<?php echo ($vo["filename"]); ?></td>
			</tr>
		</table>
		<hr/><?php endforeach; endif; ?>
	<?php echo ($page); ?>
</body>
</html>