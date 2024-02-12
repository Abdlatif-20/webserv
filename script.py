import requests

def send_requests(url):
    try:
        # Send a GET request
        response_get = requests.get(url)
        print("GET request sent successfully.")

        # Send a POST request
        # response_post = requests.post(url, data={'key': 'value'})
        # print("POST request sent successfully.")

        # Send a DELETE request
        # response_delete = requests.delete(url)
        # print("DELETE request sent successfully.")

    except requests.exceptions.RequestException as e:
        print("Error:", e)

if __name__ == "__main__":
    url = input("Enter the URL to test: ")
    send_requests(url)
