include nemu/scripts/git.mk

default:
	@echo "Please run 'make' under any subprojects to compile."

submit:
	git gc

.PHONY: default submit
