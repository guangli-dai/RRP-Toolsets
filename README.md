# RRP-Toolsets
The task-level and resource-level tools based on Regularity-based Resource Partitioning (RRP) model
The current version is v0.8. It includes the following tools.
## Resource Specification Generator (RSG)
RSG takes in the information of a task set that will be deployed on a partition. It generates an availabilit factor (a resource specification) so that the task set given is schedulable on the partition with the availability factor. 
## Cyclic Schedule Generator (CSG)
CSG generates one cyclic schedule for each CPU with the configurations of the partitions (resource specifications) taken as inputs.
## Schedualbility Test (ST)
ST can judge whether the current configuration of a partition can offer real-time performance guarantees to the task set being deployed.

To run the application, you need to install Qt 5.12 or above. A packaged application without preliminary installs required will be added soon.