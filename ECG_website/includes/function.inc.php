<?php
function emptyInputSignup($name , $email , $username , $pwd , $pwdRepeat){
	$result;
	if(empty($name) || empty($email) || empty($username) || empty($pwd) || empty($pwdRepeat)){
	$result = true ;
 }
else{
	$result = false ;
}
   return $result;
}


function invalidUid($username){
	$result;
	if(!preg_match("/^[a-zA-Z0-9]*$/" , $username)){
	$result = true ;
 }
else{
	$result = false ;
}
   return $result;
}

function invalidEmail($email){
	$result;
	if(!filter_var($email, FILTER_VALIDATE_EMAIL)){
	$result = true ;
 }
else{
	$result = false ;
}
   return $result;
}

function pwdMatch($pwd , $pwdRepeat){
	$result;
	if($pwd !== $pwdRepeat){
	$result = true ;
 }
else{
	$result = false ;
}
   return $result;
}

function uidExists($conn , $username , $email){
	$sql = "SELECT * FROM users WHERE usersUid = ? OR usersEmail = ?;";//? is a placeholder

	$stmt = mysqli_stmt_init($conn);//initialize a prepared statement

	//run this statement to the database and see is there any kind of error or not
    if (!mysqli_stmt_prepare($stmt , $sql)) {
    	header("location: ../signup.php?error=stmtfailed");
		exit();	
    }
    
    //if that doesn't fail the user details will be added now, the data from user will be passed now to the database
    mysqli_stmt_bind_param($stmt , "ss" , $username , $email ); //ss means two strings
    mysqli_stmt_execute($stmt);//excute the statement

    $resultData = mysqli_stmt_get_result($stmt);

    if ($row = mysqli_fetch_assoc($resultData)) {
    	return $row ;
    }
    else{
    	$result = false;
    	return $result;
    }

    mysqli_stmt_close($stmt);
}
 
 function createUser($conn , $name , $email ,$username , $pwd){
	$sql = "INSERT INTO users(usersName , usersEmail , usersUid ,usersPwd ) VALUES( ? , ? , ? , ?)";//? is a placeholder

	$stmt = mysqli_stmt_init($conn);//initialize a prepared statement

	//run this statement to the database and see is there any kind of error or not
    if (!mysqli_stmt_prepare($stmt , $sql)) {
    	header("location: ../signup.php?error=stmtfailed");
		exit();	
    }

    //if any hacker enter in the database,the password hashing will confuse him

    $hashedPwd = password_hash($pwd , PASSWORD_DEFAULT);
    
    //if that doesn't fail the user details will be added now, the data from user will be passed now to the database
    mysqli_stmt_bind_param($stmt , "ssss" , $name , $email , $username , $hashedPwd); //ss means two strings
    mysqli_stmt_execute($stmt);//excute the statement

    mysqli_stmt_close($stmt);
    if (!mysqli_stmt_prepare($stmt , $sql)) {
    	header("location: ../signup.php?error=none");
		exit();    
}
}

//login functions:
function emptyInputLogin($username , $pwd){
    $result;
    if(empty($username) || empty($pwd)){
    $result = true ;
 }
else{
    $result = false ;
}
   return $result;
}

function loginUser($conn, $username, $pwd){
    $uidExists = uidExists($conn , $username , $username) ;
    if ($uidExists === false) {
        header("location: ../login.php?error=wronglogin");
        exit();
    }

    $pwdHashed = $uidExists["usersPwd"];

    $checkPwd = password_verify($pwd , $pwdHashed);

    if ($checkPwd === false){
        header("location: ../login.php?error=wronglogin");
        exit();
    }
    else if ($checkPwd === true){
       session_start();
       $_SESSION["userid"] =  $uidExists["usersId"];
       $_SESSION["useruid"] =  $uidExists["usersUid"]; 
       header("location: ../home.php");
       exit();
    }
}
?>