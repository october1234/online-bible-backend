# Must be run manually

docker load -i online-bible.tar
docker run --network nginx_proxied --name online-bible online-bible
