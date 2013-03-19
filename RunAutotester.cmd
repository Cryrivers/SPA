@echo off
Autotesting\SPA-Autotester.exe Autotesting\CS3201RegressionTestCode.c Autotesting\CS3201RegressionTestQuery.txt NUL > result.txt
pause
Autotesting\SPA-Autotester.exe Autotesting\CS3202-Test1Code.c Autotesting\CS3202-Test1QueryLatest.txt result.xml >> result.txt
pause