if document.getElementsByTagName("body")[0].className.indexOf("xp") > -1
  download = document.getElementById("download")
  downloadAlt = document.getElementById("download-alt")
  screenshot = document.getElementById("screenshot")

  download.href = "/download-xp"
  downloadAlt.href = "/download"
  screenshot.src = "http://i.imgur.com/1gSL0LE.png"