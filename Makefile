.PHONY: \
	clean \
	gen \
	release_gen \
	build \
	bench \
	test \
	main \
	format

.DEFAULT_GOAL := main

BUILD_DIR=./build

clean:
	rm -rf ${BUILD_DIR} && mkdir ${BUILD_DIR}

gen:
	cmake -Wno-dev \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBoost_NO_BOOST_CMAKE=ON \
		-DASAN=1 \
		-B ${BUILD_DIR}

release_gen:
	cmake -Wno-dev \
		-DCMAKE_BUILD_TYPE=Release \
		-DBoost_NO_BOOST_CMAKE=ON \
		-B${BUILD_DIR}

build:
	make -j4 -C ${BUILD_DIR}

bench: build
	python3 stash/run_all_benchmarks.py benchmarks build

test: build
	ctest --test-dir ${BUILD_DIR}

main: build
	@echo "\033[0;32m-- Running ${BUILD_DIR}/main\033[0m" && \
	${BUILD_DIR}/main

format:
	./stash/format.sh
