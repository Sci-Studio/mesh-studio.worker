import { ServiceBusClient } from "@azure/service-bus";
import { env } from "./config/env.js";

const sb = new ServiceBusClient(env.AZURE_SERVICE_BUS_CONNECTION_STRING);
const receiver = sb.createReceiver(env.AZURE_SERVICE_BUS_QUEUE_NAME);

receiver.subscribe({
    processMessage: async (msg) => {
      console.log(msg.body);
    },
    processError: async (args) => {
      console.error("Service Bus error", args.error);
    },
  });