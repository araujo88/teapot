#!/bin/bash

# Define colors for terminal output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'  # No Color
N=0

# Define base URL of your webserver
BASE_URL="http://localhost:8888"

# Define endpoints to test (modify as needed)
ENDPOINTS=(
  "/"
  "/example"
  "/test"
  "/42"
  "/.."
)

EXPECTED_RESULT=(
  200
  200
  200
  404
  200
)

# Loop through each endpoint

for i in "${!ENDPOINTS[@]}"; do # Iterate over indices of the array
  endpoint="${ENDPOINTS[$i]}"
  expected_code="${EXPECTED_RESULT[$i]}"
  full_url="$BASE_URL$endpoint"
  response=$(curl -sSL -o /dev/null -w "%{http_code}" "$full_url")

  if [[ $response == $expected_code ]]; then
    echo -e "$N - ${GREEN}OK${NC} - GET $full_url"
  else
    echo -e "$N - ${RED}FAILED${NC} - GET $full_url (HTTP code: $response)"]
    exit 1
  fi
  N=$((N+1))
done

declare -A TEST_CASES=(
    ["/:POST"]="name=TestUser&email=test@example.com"  # Create a user
    ["/:PUT"]="name=UpdatedName"                    # Update user 123
    ["/:PATCH"]="status=published"               # Partially update article 456
    ["/:DELETE"]=""
)

for item in "${!TEST_CASES[@]}"; do
  endpoint=$(echo $item | cut -d ':' -f 1)
  method=$(echo $item | cut -d ':' -f 2)
  data=$(echo ${TEST_CASES[$item]})
  full_url="$BASE_URL$endpoint"

  response=$(curl -sSL -o /dev/null -w "%{http_code}" -X $method -d "DATA" "$full_url")

  if [[ $response == 405 ]]; then
    echo -e "$N - ${GREEN}OK${NC} - $method $full_url"
  else
    echo -e "$N - ${RED}FAILED${NC} - $method $full_url (HTTP code: $response)"
    exit 1
  fi
  N=$((N+1))
done
