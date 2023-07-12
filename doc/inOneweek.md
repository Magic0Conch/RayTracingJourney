## powershell的默认编码方式和cmd不同
cmd：`.\Raytracing.exe > image.ppm`
powershell:`.\Raytracing.exe | Out-File -Encoding ASCII image.ppm
`

