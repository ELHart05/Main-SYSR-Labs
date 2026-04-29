# SYSR - Lab 03 RMI
# Requires Java 11+
#
# Hello example (registry on port 1099):
#   make hello-server   # terminal 1
#   make hello-client   # terminal 2
#
# Calculator example (registry on port 2099):
#   make calc-server    # terminal 1
#   make calc-client    # terminal 2

JAVAC = javac
JAVA  = java

# ---- Hello ----
hello-compile:
	$(JAVAC) -d out hello/Information.java hello/InformationImpl.java \
	         hello/LanceServeur.java hello/LanceClient.java

hello-server: hello-compile
	$(JAVA) -cp out hello.LanceServeur

hello-client: hello-compile
	$(JAVA) -cp out hello.LanceClient

# ---- Calculator ----
calc-compile:
	$(JAVAC) -d out calculator/CalculatorInterface.java \
	         calculator/CalculatorImpl.java \
	         calculator/CalculatorServer.java \
	         calculator/CalculatorClient.java

calc-server: calc-compile
	$(JAVA) -cp out CalculatorServer

calc-client: calc-compile
	$(JAVA) -cp out CalculatorClient

clean:
	rm -rf out

.PHONY: hello-compile hello-server hello-client \
        calc-compile calc-server calc-client clean
