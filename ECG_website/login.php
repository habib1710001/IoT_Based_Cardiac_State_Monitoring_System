<!DOCTYPE html>
<html>
<head>
	<title>Log In</title>
	<style>
     body  {
     background-image: url("login_cover.png");
     background-repeat: no-repeat; 
     background-size: cover;

    }
    form{
        margin-left: 30%;
    }
    button{
        background-color: Red;
        border: none;
        color: white;
        text-align: center;
        text-decoration: none;
        font-size: 16px;
        margin: 4px 2px;
        padding: 8px 32px;
        border-radius : 3px;
        cursor: pointer;
    }
    button:hover {
     box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24),0 17px 50px 0 rgba(0,0,0,0.19);
    }
    input{
        border-radius : 3px;
        border-width: 0.5px;
        border-color: LightGray;
        width: 20%;
        height: 30px;
    }
    h1{
       margin-left: 30%; 
    }
    p{
        margin-left: 30%;
        color: Red ;
        font-weight:bolder;
    }
    h4{
       margin-left: 30%; 
    }
    a{
        color: black;
    }
   </style>
</head>
<body>
	<h1>𝐋𝐨𝐠 𝐈𝐧</h1>
	<p>Haven't joined? <a href="http://kusumtara.com/signup.php">Sign up</a> to connect to the technology...</p>
	<section>
		<form action = "includes/login.inc.php" method="post">
			<input type="text" name="uid" placeholder="Username/email...">
			<input type="password" name="pwd" placeholder="password...">
			<button type="submit" name="submit">Log In</button>
		</form>
	</section>
<?php 
if (isset($_GET["error"]))
		{
			if($_GET["error"] == "emptyinput"){
				echo "<p>Fill all the requirements</p>";
			}
			else if ($_GET["error"] == "wronglogin"){
				echo "<p>Invalid password or email</p>";
			}
			
		} 
 ?>
	


</body>
</html>