Creating and Running OpenTREP in Docker Containers
==================================================

# Building the Docker container

```bash
$ docker build -t opentrep/search-travel:legacy --squash .
```

# Running the Docker container
A few less options may be necessary (for instance, `--privileged` may be an
overkill); nevertheless, the following works wrt SystemD (_e.g._, for MariaDB):
```bash
$ docker run --privileged --tmpfs /run --tmpfs /tmp -v /sys/fs/cgroup:/sys/fs/cgroup:ro --rm -it opentrep/search-travel:legacy bash -c "/usr/sbin/init"
```

When prompted to log in, the credentials are `root`/`root`.
In order to exit the container:
```bash
$ shutdown -h now
```

