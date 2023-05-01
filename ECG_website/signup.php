<!DOCTYPE html>
<html>
<head>
	<title>Sign Up</title>
	<style>
	     body  {
         background-image: url("sign_cover.png");
         background-repeat: no-repeat; 
         background-size: cover;

        } 
	    div {
             border-radius: 5px;
             padding: 20px;
             text-align : center;
            }
        input[type=text], input[type=password] {
            width: 30%;
            padding: 12px 20px;
            margin: 8px 0;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }
        button[type=submit] {
            width: 30%;
            background-color: red;
            color: white;
            padding: 14px 20px;
            margin: 8px 0;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }
        p{
           text-align : center;
           color : red;
           font-weight: bolder;
        }
        a{
        color: black;
        }

}
	</style>
</head>

<body>
	
	    <div>
	    <h1>𝐒𝐢𝐠𝐧 𝐮𝐩</h1>
		<form action="includes/signup.inc.php" method="post">
			<input type="text" name="name" placeholder="Full name..."><br>
			<input type="text" name="email" placeholder="Email..."><br>
			<input type="text" name="uid" placeholder="User name..."><br>
			<input type="password" name="pwd" placeholder="password..."><br>
			<input type="password" name="pwdrepeat" placeholder="repeat password..."><br>
			<button type="submit" name="submit">Sign Up</button>
		</form>
		</div>
		<p>Have an account already?<a href="http://kusumtara.com/login.php">Login</a>  here...</p>
 <?php 
 if (isset($_GET["error"]))
		{
			if($_GET["error"] == "emptyinput"){
				echo "<p>Fill all the requirements</p>";
			}
			else if ($_GET["error"] == "invaliduid"){
				echo "<p>Invalid User ID</p>";
			}
			else if ($_GET["error"] == "invalidemail"){
				echo "<p>Give a valid email id</p>";
			}
			else if ($_GET["error"] == "passwordsdontmatch"){
				echo "<p>Password not matched</p>";
			}
			else if ($_GET["error"] == "stmtfailed"){
				echo "<p>Something Went Wrong</p>";
			}
			else if ($_GET["error"] == "usernametaken"){
				echo "<p>Username is taken,try another</p>";
			}
			else if ($_GET["error"] == "none"){
				echo "<p>You are Signed Up</p>";
			}
		} 

 ?>
</body>
</html>