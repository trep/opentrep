Creating and Running OpenTREP in Docker Containers
==================================================

# Building the Docker container

```bash
$ docker build -t opentrep/search-travel:legacy --squash .
```

# Running the Docker container

## On Linux
```bash
$ docker run --rm -it opentrep/search-travel:legacy bash
```

## On MacOS
```bash
$ docker run --privileged -v /sys/fs/cgroup:/sys/fs/cgroup:ro --rm -it opentrep/search-travel:legacy bash
```

