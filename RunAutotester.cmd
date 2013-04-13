@echo off
Autotesting\SPA-Autotester.exe Autotesting\CS3201RegressionTestCode.c Autotesting\CS3201RegressionTestQuery.txt NUL > result1.txt
pause
Autotesting\SPA-Autotester.exe Autotesting\CS3202-Test1Code.c Autotesting\CS3202-Test1QueryLatest.txt NUL > result2.txt
pause
Autotesting\SPA-Autotester.exe Autotesting\CS3202-Test2Code.c Autotesting\CS3202-Test2Query.txt NUL > result3.txt