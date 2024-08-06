FROM node:alpine AS build

WORKDIR /app

COPY package*.json ./

RUN npm install -g @angular/cli

RUN npm install

COPY . .

RUN npm run build

# Build Stage
FROM nginx:alpine
COPY nginx.conf /etc/nginx/nginx.conf
COPY --from=build /app/dist/parsing-points-client/ /usr/share/nginx/html 

EXPOSE 80

# Start Nginx
CMD ["nginx", "-g", "daemon off;"]
