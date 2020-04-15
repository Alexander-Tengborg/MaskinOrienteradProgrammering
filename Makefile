.PHONY: clean All

All:
	@echo "----------Building project:[ basic_io - Debug ]----------"
	@cd "basic_io" && "$(MAKE)" -f  "basic_io.mk" && "$(MAKE)" -f  "basic_io.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ basic_io - Debug ]----------"
	@cd "basic_io" && "$(MAKE)" -f  "basic_io.mk" clean
