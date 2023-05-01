<?php
  
  $connection = mysqli_connect("localhost", "u422366820_kusum_tara", "kusumTara2022", "u422366820_ece_iot");
  $limit=0;
  $extlim=17;
  if($_SERVER['REQUEST_METHOD'] == 'POST'){
  	$limit=$_POST['lim'];
  }
  $sql = "SELECT * FROM SensorData ORDER BY id DESC LIMIT $limit,$extlim";
  $result = $connection->query($sql);
  ?>

<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="icon" href="icon.png">
  <title>Table</title>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.6.3/css/all.css" integrity="sha384-UHRtZLI+pbxtHCWp1t77Bi1L4ZtiqrqD80Kn4Z8NTSRyMA2Fd33n5dQ8lWUE00s/" crossorigin="anonymous">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>
	<style type="text/css">
		td,th{
			text-align:center;
		}
		img[src="https://cdn.000webhost.com/000webhost/logo/footer-powered-by-000webhost-white2.png"]{
		    display:none;
		    
		}
	</style>
</head>
<body>
    <h3 style="text-align:center; margin-left: 10%; border-radius: 0px; margin:0px;  background-color:Red; border: 2px red;border-radius: 5px; margin-left:20%; margin-right:20%; color:White;">RAW ADC DATA (16Bit)</h1>
	<a class="btn btn-success" href="table.php" style="border-radius: 0px; margin:0px;  background-color:Red; border: 2px red;border-radius: 10px;">Main</a>
	<a class="btn btn-success" href="home.php" style="border-radius: 0px;margin:0px;  background-color:Red; border: 2px red;border-radius: 10px;">Home</a>
	<table class="table table-hover table-sm">
  <thead>
    <tr>
      <th scope="col">Number</th>
      <th scope="col">Value</th>
      <th scope="col">Time</th>
    </tr>
  </thead>
  <tbody>
  	<?php
  	$cnt=$result->num_rows;
  	if ($cnt > 0) {
					while($row = $result->fetch_assoc()) {
							
						echo "<tr>";
						echo "<td>" . $row["id"] . "</td>";      //col_name
						echo "<td>" . $row["value1"] . "</td>";     //col_name
						echo "<td>" . $row["reading_time"] . "</td>";    //col_name
						echo "</tr>";		
					}
				}

	?>
    
  </tbody>
</table>
<?php
  mysqli_close($connection);
  ?>
  <form method="POST" action="table.php">
  	<input type="hidden" name="lim" value="<?php echo $GLOBALS['limit']+18; ?>">
  	<input type="submit" class="btn btn-success" name="submit" value="Next" style="margin-left: 48%; background-color:Red; border: 2px red;border-radius: 10px;">
  	
   </form>


</body>
</html>
