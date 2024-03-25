# TODOs

- [X] 1. **Implement Efficient Parsing:**
   - Optimize `parseRequest` to minimize string operations and allocations.
   - Ensure buffer sizes are appropriately allocated to avoid overflow and unnecessary reallocations.

- [ ] 2. **Enhance File Serving:**
   - Preload commonly accessed files into memory to serve them faster.
   - Implement file caching strategies to reduce disk I/O.

- [ ] 3. **Concurrency and Threading:**
   - Review thread creation in `runServer` to ensure efficient handling of concurrent connections.
   - Consider using a thread pool to limit the overhead of thread creation and destruction.

- [ ] 4. **Optimize Network Operations:**
   - Evaluate and optimize the use of `sendData` and `receiveData` for minimal latency.
   - Investigate the use of non-blocking I/O or asynchronous I/O to enhance performance.

- [ ] 5. **Middleware Optimization:**
   - Review the necessity and efficiency of each middleware (`sanitizer_middleware`, `cors_middleware`, `security_middleware`).
   - Ensure middleware does not significantly impact request handling times.

- [ ] 6. **Error and Exception Handling:**
   - Optimize error handling paths to ensure minimal impact on performance.
   - Ensure exceptions like `FileNotFoundException` are efficiently caught and handled.

- [ ] 7. **Logging:**
   - Assess the performance impact of logging operations.
   - Implement asynchronous logging or reduce logging verbosity in production.

- [ ] 8. **Static File Directory Management:**
   - Review the management of the `static_files_dir` to ensure efficient file lookup.
   - Implement a more efficient data structure for routing (`this->routes`, `this->json_responses`, `this->html_responses`).

- [ ] 9. **Content-Type Determination:**
   - Optimize the process of determining the content type of responses to reduce string comparisons.

- [ ] 10. **Configuration and Scalability:**
    - Implement configuration options for scalability, like adjusting `max_connections` dynamically based on load.
    - Review and optimize settings for `socket` to ensure optimal performance across different platforms (`__linux__`, `_WIN32`).

- [ ] 11. **Security Enhancements:**
    - Ensure the server is hardened against common vulnerabilities (e.g., directory traversal, file inclusion).
    - Review and optimize `SecurityMiddleware` for minimal performance overhead.

- [ ] 12. **Testing and Profiling:**
    - Conduct stress testing to identify bottlenecks in request handling.
    - Use profiling tools to identify and optimize performance-critical sections.

- [ ] 13. **Documentation and Maintenance:**
    - Document optimization strategies and configurations for future reference.
    - Establish a maintenance plan for regularly reviewing and updating the server’s performance.

- [ ] 14. **Compliance and Standards:**
    - Ensure the server's response headers and methods comply with HTTP standards.
    - Review and optimize the server to handle HTTP/1.1 persistent connections efficiently.

- [ ] 15. **Client-side Performance:**
    - Although the server primarily serves static content, ensure served pages are optimized for client-side performance (e.g., minification of CSS and JavaScript).
