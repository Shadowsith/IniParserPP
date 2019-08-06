all:
	@echo "Build unit tests..."
	@mkdir -p bin
	@g++ -pipe unit.cpp -lpthread -o ./bin/unit_test
run:
	@echo "Run tests..."
	@./bin/unit_test
clean:
	@rm -r ./bin
rebuild:
	@make clean
	@make all
