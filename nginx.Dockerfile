FROM nginx

WORKDIR /etc/nginx

COPY nginx.conf /etc/nginx/nginx.conf
COPY cpp/httptest /etc/nginx/static/httptest
