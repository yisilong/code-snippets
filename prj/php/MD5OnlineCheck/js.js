function read(file) {
    if(window.File && window.FileReader && window.FileList && window.Blob) {
		var file = document.getElementById('file').files[0];
		if (file.size > 1024*1024*100)
		{
			alert("100m limit !");
			return;
		}
		var reader = new FileReader();
		// If we use onloadend, we need to check the readyState.
		reader.onloadend = function(evt) {
			if (evt.target.readyState == FileReader.DONE) { // DONE == 2
				document.write(hex_md5(evt.target.result));
			};
		}
		//var blob = file.slice(start, stop + 1);
		reader.readAsBinaryString(file);
		//reader.readAsText(file);
	}
	else if(typeof window.ActiveXObject != 'undefined') { // ie
		alert("don't support IE!");
	}
}

function BinaryFile(filepath){
    var adTypeBinary=1,adTypeText=2;
    var adSaveCreateNotExist=1,adSaveCreateOverWrite=2;
    var adReadAll=-1,adReadLine=-2;
    this.path=filepath;
    this.WriteAll = function(content){
        var Stream = new ActiveXObject("ADODB.Stream");
        Stream.Type = adTypeText;
       // Stream.CharSet = "ansi";
        Stream.Open();
        Stream.WriteText(content);
        Stream.SaveToFile(this.path, adSaveCreateOverWrite);
        Stream.Close();
        Stream = null;
    }
    this.ReadAll = function(){
        var Stream = new ActiveXObject("ADODB.Stream");
		if (!Stream){
			alert('fuckyoua ');		
		}
        Stream.Type = adTypeText;
       // Stream.CharSet = "ansi";
        Stream.Open();
        Stream.LoadFromFile(this.path);
        var content = Stream.ReadText(adReadAll);
        Stream.Close();
        Stream = null;
        return content;
    }
}
// Example Code
/*
var crFolder = 'C:/Temp/cr'
var bf1=new BinaryFile(crFolder+"/PCDV0026.JPG");
var bf2=new BinaryFile(crFolder+"/PCDV0026_.JPG");
bf2.WriteAll(bf1.ReadAll());
*/