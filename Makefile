.PHONY: clean All

All:
	@echo "----------Building project:[ lab2 - Debug ]----------"
	@cd "lab2" && "$(MAKE)" -f  "lab2.mk" && "$(MAKE)" -f  "lab2.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ lab2 - Debug ]----------"
	@cd "lab2" && "$(MAKE)" -f  "lab2.mk" clean
