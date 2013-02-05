@echo off
@echo Regression Testing (CS3201)
Autotesting\SPA-Autotester.exe Autotesting\CS3201RegressionTestCode.c Autotesting\CS3201RegressionTestQuery.txt NUL
pause
@echo CS3202 Testing 1
Autotesting\SPA-Autotester.exe Autotesting\CS3202-Test1Code.c Autotesting\CS3202-Test1Query.txt NUL
pause