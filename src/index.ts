import { ServiceBusClient } from "@azure/service-bus";
import { config } from "./config/env";

const sb = new ServiceBusClient(config.AZURE_SERVICE_BUS_CONNECTION_STRING);
const receiver = sb.createReceiver(config.AZURE_SERVICE_BUS_QUEUE_NAME);

receiver.subscribe({
    processMessage: async (msg) => {
      console.log(msg.body);
    },
    processError: async (args) => {
      console.error("Service Bus error", args.error);
    },
  });