<?php
	if (isset($_POST) && isset($_POST['value']))
	{
		setcookie('value', $_POST['value'], time() + (86400 * 30), "/"); // 86400 = 1 day
		header("Refresh:0");
	}
?>

<html>
<body>
	<?php
		if(!isset($_COOKIE['value'])) {
			echo "Cookie named 'value' is not set!, please fill it:";
			echo '<form action="/index.php" method="post">';
			echo '		<label for="fname">value:</label>';
			echo '		<input type="text" id="fname" name="value"><br><br>';
			echo '		<input type="submit" value="Submit">';
			echo '</form>';
		} else {
		  echo "Cookie 'value' is set!<br>";
		  echo "Value is: " . $_COOKIE['value'];
		}
	?>
</body>
</html>
