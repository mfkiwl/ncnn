// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "fuse_module_pass.h"

namespace pnnx {

class Embedding : public FuseModulePass
{
public:
    const char* match_type_str() const
    {
        return "__torch__.torch.nn.modules.sparse.Embedding";
    }

    const char* type_str() const
    {
        return "nn.Embedding";
    }

    void write(Operator* op, const TorchGraphProxy& graph, const TorchModuleProxy& mod) const
    {
        const TorchNodeProxy* embedding = graph.find_node_by_kind("aten::embedding");

        const TorchTensorProxy& weight = mod.attr("weight");

        op->params["num_embeddings"] = weight.size(0);
        op->params["embedding_dim"] = weight.size(1);

        // op->params["padding_idx"] = embedding->namedInput("padding_idx");
        // op->params["scale_grad_by_freq"] = embedding->namedInput("scale_grad_by_freq");
        op->params["sparse"] = embedding->namedInput("sparse");

        op->attrs["weight"] = weight;
    }
};

REGISTER_GLOBAL_PNNX_FUSE_MODULE_PASS(Embedding)

} // namespace pnnx
