<?php
echo "Input file name, if you want to finish, Input 0."."\n";
$count = 0;
while(true){
fscanf(STDIN, "%s", $file_name);
if($file_name == "0"){
  exit;
}
$filepath = "Ad".strval($count);
$fp = fopen($filepath, "w");
$count++;

if(is_file($file_name)){
  $text = fopen($file_name,'r');
  $data;
  $val = "i_";
  //$val = 'a';
  $cnt = 1;
  $start = FALSE;
  for($line = 1 ; !feof($text) ; $line++){
    $lines = fgets($text);
    if($lines){
      $lines = ltrim($lines);

      // ������ʸ��� /* */
      if(preg_match("/\/\*/", $lines)){
	$start = TRUE;
	continue;
      }
      if(preg_match("/\*\//", $lines)){
	$start = FALSE;
	continue;
      }      
      if($start == TRUE) continue;      
      
      // ������ʸ��� //
      $lines = mb_ereg_replace("\/\/.*\z", "\n", $lines);
      if(empty($lines)){
	continue;
      }
      
      if(preg_match("/#include/", $lines)){ // #include �ξ��
	//echo $lines;
	//@fwrite($fp, $lines);
	continue;
      }
      
      $elements = preg_split("/(\W)/",$lines,-1,PREG_SPLIT_DELIM_CAPTURE);
      // ʸ�����not word ��Ƕ��ڤ�
      
      for($i = 0 ; $i < count($elements) ; $i++){
	// ͽ���ʤ餽�Τޤ�
	if(isReservedWord($elements[$i]) == TRUE){
	  continue;
	}
	
	// printf����%d, \n �ʤɤϤ��Τޤ�
	else if(preg_match("/printf|scanf/", $lines) && $elements[$i-1] == "%" || $elements[$i-1] == "\\") continue;
	
	// �ѿ����ؿ�̾�ʤɤ��ѹ�����
	else if(preg_match("/\S/", $elements[$i])){
	  if(@array_key_exists($elements[$i], $data) == FALSE){
	    if(preg_match("/\w/", $elements[$i])){
	      if(preg_match("/\d/", $elements[$i])) continue;
	      //$data[$elements[$i]] = $val++;
	      $data[$elements[$i]] = $val.strval($cnt);
	      $cnt++;
	      $elements[$i] = $data[$elements[$i]];
	    }
	  }
	  else{
	    if(isReservedWord($elements[$i]) == FALSE){
	      $elements[$i] = $data[$elements[$i]];
	    } 
	  }
	}
      }
      // ����
      for($i = 0 ; $i < count($elements) ; $i++){
	if($elements[$i] != ' '){
	//echo $elements[$i];
	@fwrite($fp, $elements[$i]);
}
      
}
    }
  }
  fclose($text);
}else{
  print 'no file'."\n";
  exit;
}
}

// ͽ��줫Ƚ�ꤹ��
function isReservedWord($s){
  if(preg_match("/main|void|char|short|int|long|float|double|bool|catch|class|delete|using|namespace|std|auto|static|const|signed|unsigned|extern|volatile|register|return|goto|if|else|switch|case|default|break|for|while|do|continue|typedef|struct|enum|union|scanf|printf|puts|EOF|sizeof|define|true|false|argc|argv|cin|cout|iterator|string|vector|map|set|memset/", $s)){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

?>
      
      