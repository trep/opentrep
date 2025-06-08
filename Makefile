
# Today's date
EXTRACT_DATE := $(shell date '+%Y%m%d')
TODAY_DATE := $(shell date '+%Y-%m-%d')

isset-%: ## Test if variable % is set or exit with error
	echo # @: $(if $(value $*),,$(error Variable $* is not set))

.DEFAULT_GOAL:=help

# Cleaners
clean-caches: ## Clean the project caches.
	rm -f .coverage
	rm -rf .hypothesis/
	rm -rf .mypy_cache/
	rm -rf .pytest_cache/

clean-python: ## Clean the Python caches.
	find . -type f -name '*.py[co]' -delete
	find . -type d -name __pycache__ -delete

clean-poetry: ## Clean Poetry environment
	rm -f poetry.lock

clean-python-cmake: ## Clean SciKit-Build
	rm -rf _skbuild/

clean-package: ## Clean the project package.
	rm -rf dist/

clean-install: ## Clean the project installation.
	rm -rf .venv/

clean-reports: ## Clean the project generated reports.
	rm -rf reports/*.*

cleaners: clean-caches clean-python clean-python-cmake clean-poetry clean-package clean-install clean-reports ## Run all the cleaners

# Installers
init-poetry: clean-package clean-install clean-python clean-poetry ## Re-initialize Poetry dependencies
	poetry lock

install-dev: ## Install the project in dev mode
	python -mpip install -U poetry-plugin-export
	poetry lock #--no-update
	poetry run python -mpip install -U pip
	poetry run python -mpip install -U poetry-core setuptools wheel scikit-build cmake ninja protobuf
	poetry install --all-extras

install-local: ## Install the project in the local Python environment
	@python -mpip install --force-reinstall \
		dist/opentrep-*.whl

uninstall-local: ## Uninstall the project from the local Python environment
	python -mpip uninstall -y pyopentrep

# Bumpers
bump-package-project: ## Bump the package version of the project.
	sed -i.bak 's/^version = .*/version = "$(shell cat VERSION)"/' pyproject.toml && rm pyproject.toml.bak


# Builders
build-deterministic-wheel: clean-package ## Build the software artifact (wheel)
	poetry run python3 -m pip install --upgrade build
	poetry export --without-hashes --without-urls -f requirements.txt --output requirements.txt
	poetry run python3 -m build --wheel

builders: build-deterministic-wheel ## Run all the builders


